import pi3d
class EA3D:
    def __init__(self):
        # setup 3d
        self.backgroundRed = 0.0
	self.backgroundGreen = 0.0 
	self.backgroundBlue = 0.0
        self.backgroundAlpha = 1.0

        self.DISPLAY = pi3d.Display.create(frames_per_second = 25,background = (0.0, 0.0, 0.0, .0))
        mylight = pi3d.Light(lightpos=(-2.0, -1.0, 10.0), lightcol=(1.0, 1.0, 0.8), lightamb=(0.25, 0.2, 0.3))

        shader = pi3d.Shader("uv_light")
        flatshader = pi3d.Shader("uv_flat")
        #self.mysphere = pi3d.ImageSprite("camera.PNG", shader, w=10.0, h=10.0) # path relative to program dir
        #sprite = pi3d.Cuboid(3.,3.,3.) # Sphere("earth", 0.0, 0.0, -10.0, 3.0, 24)
	#sphereTexture = pi3d.Texture("camera.PNG")
        #self.mysphere = pi3d.Sphere(radius=10, sides=24, slices=24, name="sphere", x=0, y=0, z=5)
        self.mysphere  = pi3d.Model(file_string='inverseSphere.obj', name='sphere', x=0, y=-1, z=15,
                sx=2., sy=2., sz=2.)
	self.mysphere.set_shader(flatshader)
	#self.mysphere.set_draw_details(flatshader,[sphereTexture])
        #mysphere = pi3d.Model(file_string='../../pi3dDemos/models/cow2.obj', name='napf', x=0, y=-1, z=40,
        self.objects = []
        self.objects.append([])
        self.objects.append([])
        self.scene = 2
        self.scale = 2.
        dorn = pi3d.Model(file_string='dorn.obj', name='napf', x=0, y=-1, z=-40,
                sx=2., sy=2., sz=2.)
        dorn.set_shader(shader)
        self.objects[0].append(dorn)
        self.count = 15
        for i in range(self.count):
            clony=dorn.clone()
            clony.position(i,-1,-40)
            self.objects[0].append(clony)

        miene  = pi3d.Model(file_string='guteMiene.obj', name='miene', x=0, y=-1, z=-40,
                sx=2., sy=2., sz=2.)
        miene.set_shader(shader)
        self.objects[1].append(miene)
        for i in range(self.count):
            clony=miene.clone()
            clony.position(i,-1,-40)
            self.objects[1].append(clony)
            #mysphere.reparentTo(anchor)
            #clony.reparentTo(mysphere)
    
        self.xloc = 1.0
        self.yloc = 1.0
        self.rotationX = 0.0
        self.rotationY = 0.0
        self.rotationZ = 0.0
        self.vrcamxloc = 1.0
        self.vrcamyloc = 1.0
    def posX(self, x):
        self.xloc = x
        for o in self.objects[0]:	
		o.position(self.xloc,self.yloc, 15.0)

        for o in self.objects[1]:	
		o.position(self.xloc, self.yloc, 15.0)

	self.mysphere.position(self.xloc, self.yloc, 15.0)

    def posY(self,y):
        self.yloc = y	
        for o in self.objects[0]:	
		o.position(self.xloc, self.yloc, 15.0)
        for o in self.objects[1]:	
		o.position(self.xloc, self.yloc, 15.0)
	self.mysphere.position(self.xloc, self.yloc, 15.0)

    def cameraX(self,x):
        self.vrcamxloc = x
        from pi3d.Camera import Camera
        vrcamera = Camera.instance()
        if vrcamera:
            vrcamera.position((self.vrcamxloc,self.vrcamyloc,1.0))

    def cameraY(self,y):
        self.vrcamyloc = y	
        from pi3d.Camera import Camera
        vrcamera = Camera.instance()
        if vrcamera:
            vrcamera.position((self.vrcamxloc,self.vrcamyloc,1.0))
	
    def rotX(self,x):
        self.rotationX = x	
        i = 1
        for o in self.objects[0]:	
	  	o.rotateIncX((self.rotationX*i)*0.001)
		i = i + 1
        i = 1
        for o in self.objects[1]:	
	  	o.rotateIncX((self.rotationX*i)*0.001)
		i = i + 1
	self.mysphere.rotateIncX((self.rotationX*i)*0.001)

    def rotY(self,y):
        self.rotationY = y
        i = 1	
        for o in self.objects[0]:	
	  	o.rotateIncY((self.rotationY*i)*0.0012)
		i = i + 1
        i = 1	
        for o in self.objects[1]:	
	  	o.rotateIncY((self.rotationY*i)*0.0012)
		i = i + 1
	self.mysphere.rotateIncY((self.rotationY*i)*0.0012)

    def rotZ(self,z):
        self.rotationZ = z
        i = 1	
        for o in self.objects[0]:	
		o.rotateIncZ((z*i)*0.00132)
		i = i + 1
        i = 1	
        for o in self.objects[1]:	
		o.rotateIncZ((z*i)*0.00132)
		i = i + 1
	self.mysphere.rotateIncZ((z*i)*0.00132)

    def setScale(self,s):
        self.scale = s	
        for o in self.objects[0]:	
		o.scale(self.scale,self.scale,self.scale)
        for o in self.objects[1]:	
		o.scale(self.scale,self.scale,self.scale)
	self.mysphere.scale(self.scale*8,self.scale*8,self.scale*8)


    def setScene(self,s):
        self.scene = s
        
    def setBackground(self,r,g,b,a):
	self.DISPLAY.set_background(r,g,b,a)
	#print "set background to "+ str(r) + " "+str(g)+ " "+str(b)+" "+str(a) 

    def oscHandler(self, addr, tags, stuff, source):
        if isinstance(stuff[0],basestring):
            if stuff[0] == "posX":
                if isinstance(stuff[1],float):
                   self.posX(stuff[1])
            elif stuff[0] == "posY":
                if isinstance(stuff[1],float):
                    self.posY(stuff[1])
            elif stuff[0] == "rotationX":
                if isinstance(stuff[1],float):
                    self.rotX(stuff[1])
            elif stuff[0] == "rotationY":
                if isinstance(stuff[1],float):
                    self.rotY(stuff[1])
            elif stuff[0] == "rotationZ":
                if isinstance(stuff[1],float):
                    self.rotZ(stuff[1])
            elif stuff[0] == "scale":
                if isinstance(stuff[1],float):
                    self.setScale(stuff[1])
            elif stuff[0] == "backgroundRed":
                if isinstance(stuff[1],float):
		    self.backgroundRed = stuff[1]
                    self.setBackground(self.backgroundRed, self.backgroundGreen, self.backgroundBlue, self.backgroundAlpha)
            elif stuff[0] == "backgroundGreen":
                if isinstance(stuff[1],float):
		    self.backgroundGreen = stuff[1]
                    self.setBackground(self.backgroundRed, self.backgroundGreen, self.backgroundBlue, self.backgroundAlpha)
            elif stuff[0] == "backgroundBlue":
                if isinstance(stuff[1],float):
		    self.backgroundBlue = stuff[1]
                    self.setBackground(self.backgroundRed, self.backgroundGreen, self.backgroundBlue, self.backgroundAlpha)
            elif stuff[0] == "backgroundAlpha":
                if isinstance(stuff[1],float):
		    self.backgroundAlpha = stuff[1]
                    self.setBackground(self.backgroundRed, self.backgroundGreen, self.backgroundBlue, self.backgroundAlpha)
            elif stuff[0] == "camera":
                if isinstance(stuff[1],basestring):
                    if stuff[1] == "posX":
                        if isinstance(stuff[2],float):
                            self.cameraX(stuff[2])
                    if stuff[1] == "posY":
                        if isinstance(stuff[2],float):
                            self.cameraY(stuff[2])
            elif stuff[0] == "scene":
                if isinstance(stuff[1],int):
                    self.setScene(stuff[1])
