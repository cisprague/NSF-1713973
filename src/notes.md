# Notes

## Perception

1. Using as a controller a neural network which is only capable of perceiving a dynamical system's state with respect to the reference frame, where the system's dynamics are governed by the influence of other entities, which are also in reference to the reference frame, is to learn to the dynamics of the environment evolve over time. In essence this is taking action in a world which is not seen, that is knowing how the world evolves without *seeing* it.
2. Indeed, supplying also the states of ancillary entities whose influences affect a dynamical system's evolution gives more knowledges about the dynamics of the environment in which an agent is acting
3. From a cognitive perspective, if one had knowledge of which actions were taken at which times, it would mean nothing in the context of an autonomous dynamical system. A system whose dynamics evolve according it's state $\dot{\pmb{x}} = \pmb{f}(\pmb{x})$ deduces no meaning of time, and hence an agent acting under such dynamics would not benefit from knowledge of time.

## Optimal control

1. Traditionally, within the direct transcription methodology, a trajectory is discretised with respect to the control or both the control and the state. Often the problem becomes that the size of the discretisation becomes infeasibly large when a system evolves slowly over time with respect to it's controls. Hence it is desired consider a system as $\dot{\pmb{x}} = \pmb{f}(\pmb{x}, \pmb{u}(\pmb{x})) $, and to find its state feedback control function $\pmb{u}(\pmb{x})$.
2. Such a function, $\pmb{u}(\pmb{x})$ can be represented by a nonlinear function, namely a neural network.
