CXX=g++
CXXFLAG= -ggdb
CXXFWK= -framework foundation -framework OpenGL -framework GLUT -lGLEW

# Marko - Update these for your build
CXXLIB= -L/usr/local/Cellar/glew/1.9.0/lib/
CXXINC= -I/usr/local/include/GL
OBJ= OBJLoader.o ShaderSetup.o Face.o


main: $(OBJ)
	$(CXX) $(CXXFLAG) -o Bloom main.cpp OBJLoader.o ShaderSetup.o Face.o $(CXXLIB) $(CXXINC) $(CXXFWK)


OBJLoader.o: OBJLoader.h
	$(CXX) $(CXXFLAG) -c OBJLoader.cpp  

ShaderSetup.o: ShaderSetup.h ShaderSetup.cpp
	$(CXX) $(CXXFLAG) -c ShaderSetup.cpp 

Face.o: Face.h Face.cpp
	$(CXX) $(CXXFLAG) -c Face.cpp 
clean:
	rm -Rf $(OBJ)
	rm -Rf Bloom
