# Perlin Lines
# drawing pretty lines over a perlin noise field. lots of tweakable parameters.
 
 drawing uses the cairo graphics library.
 not the cleanest code in the world to read, but I'm still working on getting images out of it that I like. that comes first.
 
 the code will generate a perlin noise field, then drop 'particles' into it and save their paths one by one.
 the field's value at any given point determines in which direction the particle will be pushed/accelerated.
 lines are drawn after all paths are created and saved.
 
 old lines will repel the current line, to prevent big masses of spaghetti from forming and to stop one common 
 path from being used by every single line.
 this is referred to as 'gravity' in the code; really, this is the opposite. old lines repel, not attract.
 
 parameters are changed with the massve list of #defines at the start of the file. I won't go into all of them here,
 but they're things like gravity strength, strength of the perlin field's acceleration, numer of lines, spacing of
 the start points of those lines, resolution of the output image, etc. etc.
 
 below are some examples. there are more in the 'cool ones' folder. :)

# lines stemming from both the left and right sides of the image:
![example](https://github.com/jocab1/perlin-lines/assets/104691428/bd695638-d0b9-4e9d-b333-b9436b29fba6)

# lines starting densely packed together:
![lines packed more densely](https://github.com/jocab1/perlin-lines/assets/104691428/2a73f6ed-b445-4d18-ad3e-076ec3de7b73)

# 'gravity' from old lines cranked up a bit more:
![gravity scaled up](https://github.com/jocab1/perlin-lines/assets/104691428/101a1d6c-ff99-464f-844c-481cfa6c862c)

# visualization of an example noise field:
![vector field](https://github.com/jocab1/perlin-lines/assets/104691428/378ca1da-2a9c-4f9c-ab21-7192b8db281c)

colors here range from blue to red, depending on the value of the noise at each point.
