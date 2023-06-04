# Perlin Lines
# drawing pretty lines over a perlin noise field. lots of tweakable parameters.
 
 drawing uses the cairo graphics library.
 
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
![example](https://github.com/jocab1/perlin-lines/assets/104691428/9cb6ad6d-891c-4efb-b970-96549d290bcf)

# lines starting densely packed together:
![lines packed more densely](https://github.com/jocab1/perlin-lines/assets/104691428/faf4d3b9-5193-49b4-bddb-b71d4ea8684d)

# 'gravity' from old lines cranked up a bit more:
![gravity scaled up](https://github.com/jocab1/perlin-lines/assets/104691428/b7053c86-036f-4b50-aea8-98fecf6218cc)

# visualization of an example noise field:
![vector field](https://github.com/jocab1/perlin-lines/assets/104691428/aae5e992-27f9-4fdd-8686-2b2dfc66ccc9)

colors here range from blue to red, depending on the value of the noise at each point.
