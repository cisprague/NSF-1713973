/* cheby.c - extract and use Chebyshev position-only coefficients from an SPK
   file.  This illustrates how the coefficients may be extracted once using the
   SPICE library, and then used after that without the SPICE library.

   Mark Adler   August 15, 2015   placed in the public domain
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "SpiceUsr.h"

/*
    DAF/SPK file format notes:

    ic[0] - target code
    ic[1] - center code
    ic[2] - frame code
    ic[3] - representation code (2 == Chebyshev position only)
    ic[4] - initial address of array
    ic[5] - final address of array

    len = ic[5] - ic[4] + 1

    dc[0] - initial epoch of data (seconds relative to J2000)
    dc[1] - final epoch of data (seconds relative to J2000)

    seg[len-4] - initial epoch of the first record (seconds relative to J2000)
    seg[len-3] - interval length of each record (seconds)
    seg[len-2] - elements in each record
    seg[len-1] - number of records

    seg[len-1] * seg[len-2] + 4 == len
    n = seg[len-2]
    num = (n - 2) / 3

    rec[0] - midpoint of interval covered by record (seconds relative to J2000)
    rec[1] - radius of interval (seconds)
    rec[2..num+1] - X coefficients (constant term first)
    rec[num+2..2*num+1] - Y coefficients
    rec[2*num+2..n-1] - Z coefficients

    For t, evaluate the Chebyshev polynomials T_n at (t - rec[0]) / rec[1],
    multiply by the coefficients, and sum.  The derivatives of the polynomials
    can be used to compute the velocity.  See cheby_eval() here.  The results
    are in km and km/s.  Note that all times are ephemeris times, and so do not
    take into account leap seconds.

    A SpiceDouble is simply a C double.  A SpiceInt is an integer type whose
    size is half that of double, so that two SpiceInt's fit in a SpiceDouble.
 */

/* Evaluate the given Chebyshev polynomial at x, returning both the evaluated
   polynomial in *f, and the evaluated derivative of the polymonial in *df. The
   number of coefficients is num (the degree of the polynomial is num - 1), and
   the coefficients are coeff[0..num-1].  The first coefficient coeff[0] is the
   constant term.  The scaling of x is provided by the midpoint scale[0] and
   the radius scale[1].  x must fall in the range scale[0] - scale[1] to
   scale[0] + scale[1].  Outside of that range, the polynomial is not valid. */
void cheby_eval(double x, double *scale, double *coeff, long num,
                double *f, double *df)
{
    double x2, w0 = 0., w1 = 0., dw0 = 0., dw1 = 0., tmp;

    x = (x - scale[0]) / scale[1];
    x2 = x * 2.;
    while (--num) {
        tmp = dw1;
        dw1 = dw0;
        dw0 = w0 * 2. + dw0 * x2 - tmp;
        tmp = w1;
        w1 = w0;
        w0 = coeff[num] + (x2 * w0 - tmp);
    }
    *f = coeff[0] + (x * w0 - w1);
    *df = (w0 + x * dw0 - dw1) / scale[1];
}

/* Find the appropriate SPK record for time t and compute the position and
   velocity for that time.  Returns 0 on success, 1 if the time is not covered
   by the segment. */
int cheby_posvel(double t, double *seg, long len, double pos[3], double vel[3])
{
    long k, num;

    k = (long)floor((t - seg[len - 4]) /    // seg[len-4] is initial epoch
                    seg[len - 3]);          // seg[len-3] is record span
    if (k < 0 || k >= (long)seg[len - 1])   // seg[len-1] is number of records
        return 1;
    num = (long)seg[len - 2];               // seg[len-2] is size of record
    seg += k * num;                         // point seg to the record for t
    num = (num - 2) / 3;                    // number of coefficients
    cheby_eval(t, seg, seg + 2, num, pos, vel);
    cheby_eval(t, seg, seg + 2 + num, num, pos + 1, vel + 1);
    cheby_eval(t, seg, seg + 2 + 2 * num, num, pos + 2, vel + 2);
    return 0;
}

/* Verify that the provided segment meets the constraints of a uniform set of
   coefficient records.  Return 0 on success or 1 if the segment is invalid.
   This should be done before using the segment in order to avoid segfaults on
   invalid data. */
int cheby_verify(double *seg, long len)
{
    double
        recs = seg[len - 1],    // number of records
        elts = seg[len - 2],    // elements (doubles) in each record
        span = seg[len - 3],    // time span of each record in seconds
        init = seg[len - 4];    // initial epoch in seconds relative to J2000
    long n, k;
    double *p, *q;

    if (recs != (long)recs ||                       // recs is an integer
        elts != (long)elts ||                       // elts is an integer
        (long)recs * (long)elts + 4 != len ||       // total length is correct
        3 * (((long)elts - 2) / 3) + 2 != elts ||   // integer number of coeffs
        seg[0] - seg[1] != init ||                  // 1st start is init
        span != 2 * seg[1])                         // 1st radius matches span
        return 1;
    n = (long)recs;
    k = (long)elts;
    p = seg;
    while (--n) {
        q = p + k;                                  // scan all q following p
        if (q[1] != p[1] ||                         // all radii the same
            q[0] - q[1] != p[0] + p[1])             // next start is last end
            return 1;
        p = q;
    }
    return 0;
}

/* Print an error message. */
void err(char *msg, ...)
{
    fputs("cheby error: ", stderr);
    va_list ap;
    va_start(ap, msg);
    vfprintf(stderr, msg, ap);
    va_end(ap);
    putc('\n', stderr);
}

/* SPK segment descriptor. */
typedef struct {
    long target;        // target body code
    long center;        // center body code
    long frame;         // frame of reference code
    long len;           // length of segment in doubles
    double *seg;        // allocated segment
} segment_t;

/* Load one segment of an SPK file, which covers one target over a range of
   epochs.  Save the target code, reference location code for the target
   position, and the reference frame code.  Load the segment and verify its
   structure.  On success return 0.  If there is an error, return 1 and set
   s->seg to NULL. */
int cheby_segment(SpiceInt daf, SpiceDouble *dc, SpiceInt *ic, segment_t *s)
{
    SpiceDouble *last;

    // save segment codes
    s->target = ic[0];
    s->center = ic[1];
    s->frame = ic[2];

    // allocate memory for the segment and read it in
    s->len = ic[5] - ic[4] + 1;                 // number of doubles in segment
    s->seg = malloc(s->len * sizeof(SpiceDouble));
    if (s->seg == NULL) {
        err("out of memory");
        return 1;
    }
    dafgda_c(daf, ic[4], ic[5], s->seg);        // load segment
    if (failed_c()) {
        reset_c();
        free(s->seg);
        s->seg = NULL;
        err("could not read SPK segment from file");
        return 1;
    }

    // verify the integrity of the segment
    last = s->seg + s->len - 4 - (long)(s->seg[s->len - 2]);
    if (cheby_verify(s->seg, s->len) ||         // segment structure ok
        dc[0] != s->seg[s->len - 4] ||          // start epoch matches
        dc[1] != last[0] + last[1]) {           // end epoch matches
        free(s->seg);
        s->seg = NULL;
        err("SPK segment format is invalid");
        return 1;
    }

    // return loaded segment
    return 0;
}

/* Scan through the SPK file path and extract all of the Chebyshev
   position-only segments, saving them in an allocated array of segment_t,
   which is returned.  If there is an error, NULL is returned.  *segs is set to
   the number of segments in the array.  Once this is done, this array can be
   used by cheby_verify() and cheby_posvel() above, with no dependency on or
   reference to the SPICE library. */
segment_t *spk_extract(char *path, long *segs)
{
    SpiceInt daf;
    SpiceBoolean found;
    union {
        SpiceDouble d[128];
        SpiceChar c[1024];
    } sum;
    const SpiceInt nd = 2, ni = 6;
    SpiceDouble dc[nd];
    SpiceInt ic[ni];
    segment_t *spk, *mem;

    // turn off error reporting and aborts for SPICE functions
    errprt_c("set", 0, "none");
    erract_c("set", 0, "return");

    // open the file and verifiy that it is a DAF SPK file
    dafopr_c(path, &daf);                       // open SPK file for reading
    if (failed_c()) {
        reset_c();
        err("could not open %s as a DAF", path);
        return NULL;
    }
    dafgsr_c(daf, 1, 1, 128, sum.d, &found);    // read first record
    if (failed_c() || !found || memcmp(sum.c, "DAF/SPK ", 8)) {
        reset_c();
        dafcls_c(daf);
        err("%s is not an SPK file", path);
        return NULL;
    }

    // count the number of Chebyshev position-only segments in the DAF file
    *segs = 0;
    dafbfs_c(daf);                              // begin forward search
    while (daffna_c(&found), found) {           // find the next array
        dafgs_c(sum.d);                         // get array summary
        dafus_c(sum.d, nd, ni, dc, ic);         // unpack the array summary
        if (failed_c())
            break;
        if (ic[3] == 2)                         // Chebyshev position only
            (*segs)++;                          // count segment
    }
    if (failed_c() || *segs == 0) {
        reset_c();
        dafcls_c(daf);
        err("file error or Chebyshev position-only segments in %s", path);
        return NULL;
    }

    // allocate table of segment descriptors
    spk = malloc(*segs * sizeof(segment_t));
    if (spk == NULL) {
        dafcls_c(daf);
        err("out of memory");
        return NULL;
    }

    // read and save the Chebyshev position-only segments
    *segs = 0;
    dafbfs_c(daf);                              // begin forward search
    while (daffna_c(&found), found) {           // find the next array
        dafgs_c(sum.d);                         // get array summary
        dafus_c(sum.d, nd, ni, dc, ic);         // unpack the array summary
        if (failed_c())
            break;
        if (ic[3] == 2 && !cheby_segment(daf, dc, ic, spk + *segs))
            (*segs)++;
    }
    if (failed_c() || *segs == 0) {
        reset_c();
        dafcls_c(daf);
        free(segs);
        err("no valid Chebyshev position-only segments in %s", path);
        return NULL;
    }

    // close the DAF file and return segment table
    dafcls_c(daf);
    errprt_c("set", 0, "short");
    erract_c("set", 0, "abort");
    mem = realloc(spk, *segs * sizeof(segment_t));
    if (mem != NULL)
        spk = mem;
    return spk;
}

/* Free the resources of an SPK structure created by spk_extract(). */
void spk_free(segment_t *s, long n)
{
    long i;

    for (i = 0; i < n; i++)
        free(s[i].seg);
    free(s);
}

/* Load the SPK files on the command line and verify the position and velocity
   at J2000 + 0 seconds for each Chebyshev position-only segment against the
   same result from the SPICE library. */
int main(int argc, char **argv)
{
    segment_t *s;
    long n, i;
    double pos[3], vel[3];
    SpiceInt eph, frame, center;
    SpiceDouble desc[5], pv[6];
    SpiceBoolean found;
    SpiceChar id[41];

    while (++argv, --argc) {
        s = spk_extract(*argv, &n);
        if (s == NULL) {
            err("could not load %s as an SPK file", *argv);
            continue;
        }
        furnsh_c(*argv);
        for (i = 0; i < n; i++) {
            // show segment info and position and velocity at J2000 + 0
            printf("target = %ld, center = %ld, frame = %ld\n",
                   s[i].target, s[i].center, s[i].frame);
            if (s[i].seg == NULL || cheby_verify(s->seg, s->len)) {
                err("bad segment");
                putchar('\n');
                continue;
            }
            if (cheby_posvel(0, s[i].seg, s[i].len, pos, vel)) {
                err("J2000 + 0 out of range (!)");
                putchar('\n');
                continue;
            }
            printf("pos(0) = (%g, %g, %g)\n", pos[0], pos[1], pos[2]);
            printf("vel(0) = (%g, %g, %g)\n", vel[0], vel[1], vel[2]);

            // check position and velocity against SPICE library access
            spksfs_c(s[i].target, 0, sizeof(id), &eph, desc, id, &found);
            if (!found) {
                err("target %d not found!", s[i].target);
                putchar('\n');
                continue;
            }
            spkpvn_c(eph, desc, 0, &frame, pv, &center);
            if (s[i].frame != frame || s[i].center != center)
                err("codes mismatch");
            if (pos[0] != pv[0] || pos[1] != pv[1] || pos[2] != pv[2])
                err("position mismatch");
            if (vel[0] != pv[3] || vel[1] != pv[4] || vel[2] != pv[5])
                err("velocity mismatch");
            putchar('\n');
        }
        unload_c(*argv);
        spk_free(s, n);
    }
    return 0;
}
