all:
	g++ similarity.cpp -lopencv_core -lopencv_imgproc \
	-lopencv_highgui -lopencv_imgcodecs -lopencv_features2d \
	 -I /usr/include/opencv4 -o similarity