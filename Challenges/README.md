# Episode 1: The Window

Challenges:

1. Create an entry point (Main function or class) for your Minecraft Clone application.
2. Inside this entry point, create your window and start the main loop.
3. If you have multiple monitors, add support for launching the window in different monitors.
4. Add the ability to launch your window in full screen.
5. Add event handlers for mouse input and keyboard input.
   * Add event handlers for game controllers if you have one to test with (Optional)

# Episode 2: Buffers

1. Draw a square on the screen using `glDrawArrays` (Remember, store your vertex positions in normalized device coordinates which range from -1, 1 in the x and y directions).
2. Draw a square on the screen using `glDrawElements`.
3. Draw a star using `glDrawElements`.
4. Draw the outline of a square using the `GL_LINES` primitive
5. Repeat the star challenge, but use the named buffer technique we just talked about.

# Episode 3: Shaders

1. Read the book of shaders and learn how to think about shaders conceptually.
    * Create a shader abstraction in your program. This abstraction should be able to:
    * Compile a shader given the shader type (Vertex or Fragment) and the shader’s filepath.
    * Destroy a shader that has been compiled by freeing the shader object in OpenGL
2. Create a shader program abstraction. This abstraction should be able to:
    * Take two filepaths, the vertex and fragment shader, and compile and link the programs into one shader program object.
    * Bind the shader program object
    * Unbind the shader program object (possible by calling `glUseProgram(0);`)
    * Destroy the shader object on the GPU
    * Upload a uniform variable of any data type to the shader object
3. Create a program that does the following:
    * Creates a square in local coordinates and uploads it to the GPU.
    * Uploads transformation, projection and view matrix as uniform variables to transform this square from world space to screen space in the vertex shader.
    * Moves this square around the screen by modifying the transformation matrix and uploading the result to the shader uniform variable. You should be able to move the square using the WASD keys on the keyboard.
4. Create a program that does the following:
    * Creates a square in local coordinates and uploads it to the GPU.
    * Places this square in the center of the screen using special transformation, view and projection matrices.
    * Uploads the same uniform variables you use in the book of shaders tutorials.
    * Is able to run any of the programs you create in the book of shaders.
5. Once you have step 5 complete. Finish the following challenges from the book of shader tutorials:
    * Draw several circles on one square that’s in your game world using a fragment shader.
    * Create a shader that mimics a mondrian painting.
    * Create a spinning color wheel similar to a Mac’s loading cursor.
6. (OPTIONAL) Create a vertex shader that uses the perlin noise function in the book of shaders tutorial to modify a tesselated plane. To do this, do the following:
    * Create a set of 3D vertices that represent a flat plane on the CPU. Make it so that you can increase or decrease the tesselation very easily. (You can do this by simply creating a grid of squares).
    * Upload these vertices to the GPU
    * In the vertex shader, offset the height of each of these vertices using the fbm (fractal brownian motion) function from the book of shaders tutorials.
7. (OPTIONAL) Even more above and beyond challenge. Add lighting to your perlin noise terrain. To do this you can do the following:
    * You can calculate the normal of any point on the terrain by doing the following calculation: VertexNormal = normalize(vec3(leftHeight - rightHeight, 2.0, frontHeight - backHeight)); Where the leftHeight, rightHeight, frontHeight and backHeight are obtained from calculating the values of the grid points to the left, right, front and back of the current point.
    * Send these normals to the fragment shader.
    * Use an algorithm called Phong shading to simulate a light source. You can find an extensive tutorial about this here: https://learnopengl.com/Lighting/Basic-Lighting 

