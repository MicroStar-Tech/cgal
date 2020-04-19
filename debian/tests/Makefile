all: rounding_modes_simple rounding_math_simple rounding_modes_cgal

rounding_modes_simple: rounding_modes_simple.cpp
	g++ $^ -o $@ -frounding-math $(CXXFLAGS) $(LDFLAGS)

rounding_math_simple: rounding_math_simple.cpp
	g++ $^ -o $@ -frounding-math $(CXXFLAGS) $(LDFLAGS)

rounding_modes_cgal: rounding_modes_cgal.cpp
	g++ $^ -o $@ -frounding-math $(CXXFLAGS) $(LDFLAGS) -I../../include

clean:
	rm -f rounding_modes_simple rounding_math_simple rounding_modes_cgal
