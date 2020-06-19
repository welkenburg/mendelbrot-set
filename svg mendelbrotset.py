# 360p = 480 x 360
# 480p = 858 x 480 - SD
# 720p = 1280 x 720 - HDready
# 1080p = 1920 x 1080 - FullHD.
# 2160p = 3860 x 2160 - Ultra-HD / 4K

from math import log , sqrt , cos

#the image will be stocked in this file
path = "mendel.svg"

#dimentions of the picture
width = 480
height = 360

#squares (points) size
resolution = 1

#maximum iterations done for each point
imax = 100

#number for wich the loop top
P = 10000000

#point in the middle of the picture
x = -0.5
y = 0.2

zoom = 2
radius = 2 / 2 ** zoom

#constant K
k = log(2)

#color of the set
mcolor = (220,220,220)


#start of the file
file = f'<svg baseProfile="full" height="100%" version="1.1" width="100%" xmlns="http://www.w3.org/2000/svg" xmlns:ev="http://www.w3.org/2001/xml-events" xmlns:xlink="http://www.w3.org/1999/xlink"><defs />'

#return a string of the r,g,b colors in the html5 style
def rgb(color):
	return '#%02x%02x%02x' % color

def color(v):
	#this can be changed to whatever function you like
	x = log(v) / k
	a = 1 / log(2)
	b = (1 / 3 *sqrt(2)) / log(2)
	c = (1/7.3**1/8)/log(2)
	r = 255 * (1 - cos(a * x)) / 2
	g = 255 * (1 - cos(b * x)) / 2
	b = 255 * (1 - cos(c * x)) / 2
	return rgb((int(r),int(g),int(b)))

class jump(Exception):
	pass



if __name__ == "__main__":
	#going through every pixels
	for j in range(0,height,resolution):

		#print advancment in percentage
		print(f'[DRAWING]{int(100*j/height)}%') if j % 10 == 0 else 0
	
		for i in range(0,width,resolution):

			#initialize z to 0 and c to the relative coordinates x and y : c = x + yi
			c = complex(( x - radius ) +  i * radius * 2 / width , ( y - radius * height / width ) +  j * ( radius * height / width ) * 2 / height)
			z = complex(0,0)

			try:
				#iterating until imax or |z| > P
				for n in range(imax):
					if abs(z) > P:
						#same as goto jump in cpp
						raise jump

					#equation that define the mandelbrot set
					z = z*z + c

				#the point is in the set : draw the pixel to the picture with the color defined in mcolor
				file += f'<rect fill="{rgb(mcolor)}" height="{resolution}" width="{resolution}" x="{i}" y="{j}" />'
				
			except jump:
				#add the pixel to the picture with the color defined using color(log(abs(z))/2 ** n)
				file += f'<rect fill="{color(log(abs(z))/2 ** n)}" height="{resolution}" width="{resolution}" x="{i}" y="{j}" />'

	#end of the svg file
	file += "</svg>"

	#write the data to a svg file specified in path
	with open(path,"w") as out:
		out.write(file)

