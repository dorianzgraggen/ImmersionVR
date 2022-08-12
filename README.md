:warning: **Read at your own risk, I do not approve of this code anymore.**

# ImmersionVR
 The C++ code of Immersion VR

## C++ classes
Overview of C++ classes

### PlayerCharacter
Easily the most important class: Handles teleportation, the material spheres, hand gestures, the tutorial

### MaterialListComponent
Is added to every object whose material can be changed. Has an array with materials, which can be changed in Unreal Engine's properties panel

### ClickableMaterial
Actor for the material spheres

### ApplyButton
Actor that is spawed when changing the materials where the user can apply the changes

### OpenDoorComponent
Was added to the doors that should open when the tutorial is done. Handles opening them.

### Lift
Handles everything that has to do with the main part of the lift (i. e. the part of the lift that moves) and playing the lift sounds

### LiftDoor
Handles opening the lift doors that do not move

### DisableOnStart
Used to destroy actos on begin play that are solely used for generating the NavMesh
