#ifndef CAMERA_H
#define CAMERA_H

class Camera {
	
	private:
		u16* mapMemory;
		u16* level;
	
	public:
		int x;
		int y;
		
		Camera(u16* level);
		void update();
	
};

#endif
