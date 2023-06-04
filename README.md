# Perlin Lines
# drawing pretty lines over a perlin noise field. lots of tweakable parameters.
 
 the code will generate a perlin noise field, then drop 'particles' into it and trace their paths one by one.
 the field's value at any given point determines in which direction the particle will be pushed/accelerated.
 
 old lines will repel the current line, to prevent big masses of spaghetti from forming and to stop one common 
 path from being used by every single line.
 this is referred to as 'gravity' in the code; really, this is the opposite. old lines repel, not attract.
 
 parameters are changed with the massve list of #defines at the start of the file. I won't go into all of them here,
 but they're things like gravity strength, strength of the perlin field's acceleration, numer of lines, spacing of
 the start points of those lines, resolution of the output image, etc. etc.
 
 below are some examples. there are more in the 'cool ones' folder. :)
 
![example](https://github.com/jocab1/perlin-lines/assets/104691428/9cb6ad6d-891c-4efb-b970-96549d290bcf)
![lines packed more densely](https://github.com/jocab1/perlin-lines/assets/104691428/faf4d3b9-5193-49b4-bddb-b71d4ea8684d)
![gravity scaled up](https://github.com/jocab1/perlin-lines/assets/104691428/b7053c86-036f-4b50-aea8-98fecf6218cc)
