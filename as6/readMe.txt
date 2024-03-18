git submodule add https://github.com/joshuadahlunr/raylib-cpp.git



For Compiling: 

Delete Build folder

mkdir build> cd build > cmake > make 







To run: 

(in the build directory)

./as6

enumarate controls: 

w: speed up
s: slow down 
a: turn right 
d: turn left
space: stop moving selected entity
tab: Switch selected entity 

KNOWN PROBLEMS: 

- bounding box does not follow the entity around, but does switch 
- Transformations are not working as intended, the plane rotates only a tiny bit, but does move in the direction of the heading, the model just isnt rotating 


Extra Credit: 
- added music 
