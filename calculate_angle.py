#motor gives the displacement of 0.0254mm per step.

#

import math

Onestepdist = 0.0254
dist2motor = 20.42
microstepping = 16 * 10000

def dist_to_steps(dist):
    steps_from_dist = round(float(int(round((dist/Onestepdist) * microstepping))//1000), 2)
    # print(steps_from_dist)
    return steps_from_dist
# def angle_to_steps(angle):

#     displacement = math.tan(math.radians(angle))*dist2motor
#     steps_from_angle = int(round((displacement/Onestepdist) * microstepping))

#     # print(steps_from_angle)
#     return steps_from_angle
def angle_to_steps(angle):
    steps_from_angle =  round(float((angle*microstepping)/360), 2)
    return steps_from_angle

