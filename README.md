### Project 4 : Animator
##### ZHAO Lucen, FENG Haoan 	© 2017.4.x
[Project page](https://course.cse.ust.hk/comp4411/Password_Only/projects/animator03/index.html)
***

#### Requirements

* [x] (Zhaolucen) Implement the following curve types with support for wrapping:  
    - Bézier (splined together with C0 continuity)  
    - B-spline  
    - Catmull-Rom  
* [x] (Fengyee) Implement a particle system that is integrated with your model in some way.  

***  
#### Bells and Whistles

* [ ] (3) (ZhaoLucen) Enhance the required spline options. (  A maximum of 3 whistles will be given out in this category.)
    - Let the user control the tension of the Catmull-Rom spline
    - Implement higher degree polynomial splines (ones that are C3 or C4 continuous)
    - Implement one of the standard subdivision curves (e.g., Lane-Riesenfeld or Dyn-Levin-Gregory).
    - Allow the user to specify the derivatives at the two endpoints of your C2 interpolating curves.
    - Add options to the user interface to enforce C0 or C1 continuity between adjacent Bézier curve segments automatically. (It should also be possible to override this feature in cases where you don't want this type of continuity.)
    - Add the ability to add a new control point to any curve type without changing the curve at all.
* [ ] (1) (Fengyee) Extends your project so that it support the use of the transparent TGA files.
* [ ] (1) (Fengyee) Render a mirror in your scene.
* [ ] (1) (Fengyee) You can achieve more spectacular effects with a simple technique called billboarding.
* [ ] (4) (Fengyee) Use the billboarded quads you implemented above to render the following effects.  Each of these effects is worth one whistle provided you have put in a whistle worth of effort making the effect look good.
    - [ ] Fire (Fengyee) (You'll probably want to use additive blending for your particles - glBlendFunc(GL_SRC_ALPHA,GL_ONE); )
    - [ ] Snow
    - [ ] Water fountain
    - [ ] Fireworks
* [ ] (1) Use environment mapping to simulate a reflective material.
* [ ] (1) Implement a motion blur effect (example1, example2). 
* [ ] (1) Implement a skyboxes for rendering background of scene.
* [ ] (1)  Implement your simulation engine using a higher-order method such as the Runge-Kutta technique. 
* [ ] (1) Use openGL  display lists to speed up the rendering of one or more of your more complicated models.
* [ ] (1) (ZhaoLucen) Implement adaptive Bézier curve generation; 
* [ ] (2) Extend the particle system to handle springs.
* [ ] (2) Allow for particles to bounce off each other by detecting collisions when updating their positions and velocities.
* [ ] (2) Implement a "general" subdivision curve, so the user can specify an arbitrary averaging mask 
* [ ] (2) Heightfields are great ways to build complicated looking maps and terrains pretty easily.
* [ ] (2) If you have done metaballs in project 2 or 3.  Extend your metaballs so that it support environment mapping.(
* [ ] (3)  Add a lens flare.
* [ ] (3) If you find something you don't like about the interface, or something you think you could do better, change it!
* [ ] (4) Add flocking behaviors to your particles to simulate creatures moving in flocks, herds, or schools.
* [ ] (4) (ZhaoLucen) Implement a C2-Interpolating curve.  There is already an entry for it in the drop-down menu.
* [ ] (4) (ZhaoLucen) Add the ability to edit Catmull-Rom curves using the two "inner" Bézier control points as "handles" on the interpolated "outer" Catmull-Rom control points.
* [ ] (4) (ZhaoLucen) Implement picking of a part in the model hierarchy. In other words, make it so that you can click on a part of your model to select its animation curve. 
* [ ] (4) If you implemented twist for your original model, the camera movement for your old modeler can give some unexpected results.
* [ ] (4)  extend the basic requirements so that it can draw bspline surface in the 3D scene.
* [ ] (6)  Implement projected textures
* [ ] (6) An alternative way to do animations is to transform an already existing animation by way of motion warping
* [ ] (6) Implement Cell Shading
* [ ] (8)  rigid-body simulations
* [ ] (8) Extend your system to support subdivision surfaces.
* [ ] (4) Implement picking of a part in the model hierarchy. In other words, make it so that you can click on a part of your model to select its animation curve. 
* [ ] (4) If you implemented twist for your original model, the camera movement for your old modeler can give some unexpected results.
* [ ] (4) (ZhaoLucen) extend the basic requirements so that it can draw bspline surface in the 3D scene.
* [ ] (6)  Implement projected textures
* [ ] (6) An alternative way to do animations is to transform an already existing animation by way of motion warping
* [ ] (6) Implement Cell Shading
* [ ] (8)  rigid-body simulations
* [ ] (8) Extend your system to support subdivision surfaces.

* [ ] (16) Inverse kinematics
* [ ] (16) Interactive Control of Physically-Based Animation
