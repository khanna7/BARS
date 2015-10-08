Compiling:

1. Create directory such as a Release or Debug.
2. Copy Makefile.tmplt into the directory and rename to Makefile
3. cd into the created directory
4. Edit the Makefile for your setup.
5. make 

Make targets are transmission_model and clean. make with no argument
will run transmission_model

Code will compile into an X/build directory where X is the directory
created in step 1. Application binares will also be created in X.

Note that Release and Debug directories are in .gitignore so
you can do whatever you like in there. 


Running:

From within Release or Debug directory:

./transmission_model-0.0 ../r/transmission_model.R 

