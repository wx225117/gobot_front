
# from typing_extensions import runtime_checkable
import cv2
import numpy as np
from math import sin, cos

import sys
sys.path.append ("/home/pi/pylib")
from mqtt_helper import g_mqtt
from config import config


class GridFinder():
    def __init__(self, aruco_ids, area_size):
        '''   
        area_size: (width, height), unit is pix

        Supported board_types:
            Warehouse: Four aruco marks
            Commander: One aruco mark.
            Go_game_board_19x19: Normally two aruco marks
                                 Calibrate mode, Four marks
        '''
        self.__mark_ids = aruco_ids
        if area_size is not None:
            self.__area_width, self.__area_height  = area_size
        
    def __get_rid_of_useless_corners(self):
        '''
        For better performance only. But sometimes, this will cause worse performance.
        '''
        pass

    def find_corners(self, image):
        '''
        marker_ids is a list of [top_right, bottom_right, bottome_left, top_left]
        This code can not find any id which is greater than 50.
        So the maximum id can be used is 249. saying the range is 0 to 249
        '''
        arucoDict = cv2.aruco.Dictionary_get(cv2.aruco.DICT_4X4_250)
        arucoParams = cv2.aruco.DetectorParameters_create()
        corners, ids, rejected = cv2.aruco.detectMarkers(image, arucoDict, parameters=arucoParams)
        # result = []
        print('double check target ids ',self.__mark_ids)
        print('found ids ', ids)
        result = []
        # verify *at least* one ArUco marker was detected
        if len(corners) >= len(self.__mark_ids):
            # get rid of useless corners

            # flatten the ArUco IDs list
            ids = ids.flatten()
            for target_id in self.__mark_ids:
                print('----------------Searching.... ', target_id)
                # loop over the detected ArUCo corners
                for (markerCorner, markerID) in zip(corners, ids):
                    if target_id == markerID:
                        print('got matched id', target_id)
                        # extract the marker corners (which are always returned in order of:
                        #   [top-left, top-right, bottom-right, and bottom-left]
                        corners2 = markerCorner.reshape((4, 2))
                        topLeft, topRight, bottomRight, bottomLeft = corners2
                        # convert each of the (x, y)-coordinate pairs to integers
                        topRight = (int(topRight[0]), int(topRight[1]))
                        bottomRight = (int(bottomRight[0]), int(bottomRight[1]))
                        bottomLeft = (int(bottomLeft[0]), int(bottomLeft[1]))
                        topLeft = (int(topLeft[0]), int(topLeft[1]))    

                        # compute and draw the center (x, y)-coordinates of the ArUco marker
                        cX = int((topLeft[0] + bottomRight[0]) / 2.0)
                        cY = int((topLeft[1] + bottomRight[1]) / 2.0)

                        # print('markid=', markerID, 'center=', (cX, cY),topLeft, bottomRight, bottomLeft, topLeft)
                        result.append ([cX,cY])

                        # print("[INFO] ArUco marker ID: {}".format(markerID))

                        if True:
                            # draw the bounding box of the ArUCo detection
                            color_green = (0,255,0)
                            pen_thickness = 2
                            cv2.line(image, topLeft, topRight, color_green, pen_thickness)
                            cv2.line(image, topRight, bottomRight, color_green, pen_thickness)
                            cv2.line(image, bottomRight, bottomLeft, color_green, pen_thickness)
                            cv2.line(image, bottomLeft, topLeft, color_green, pen_thickness)

                            cv2.circle(image, (cX, cY), 4, (0, 0, 255), -1)
                            # draw the ArUco marker ID on the image
                            cv2.putText(image, str(markerID),
                                        (topLeft[0], topLeft[1] - 15), cv2.FONT_HERSHEY_SIMPLEX,
                                        8, (0, 255, 0), 2)

                            # img_marker = self.draw_axis(image,0,0,0)


                            # show the output image
                    
                            # rvec, tvec = cv2.aruco.estimatePoseSingleMarkers(corners, 0.05, mtx, dist) #Estimate pose of each marker and return the values rvet and tvec---different from camera coeficcients  
                            # (rvec-tvec).any() # get rid of that nasty numpy value array error  
                            
                            # cv2.aruco.drawAxis(image, mtx, dist, rvec, tvec, 0.1) #Draw Axis  
                            # cv2.aruco.drawAxis(image, )
                            # cv2.aruco.drawDetectedMarkers(image, corners) #Draw A square around the markers  

                            # image = cv2.aruco.drawMarker(cv2.aruco.DICT_4X4_1000,)
                            # image = self.draw_axis_2(image, corners)
            if config.publish_mqtt:
                g_mqtt.publish_cv_image('gobot_stonehouse/eye/marker', image)
        return result

    def get_perspective_view(self, img, pts):
        # specify desired output size 
        width = self.__area_width
        height = self.__area_height

        # specify conjugate x,y coordinates (not y,x)
        input = np.float32(pts)
        output = np.float32([[0,0], [width-1,0], [width-1,height-1], [0,height-1]])

        # compute perspective matrix
        matrix = cv2.getPerspectiveTransform(input,output)

        # print(matrix.shape)
        # print(matrix)

        # do perspective transformation setting area outside input to black
        imgOutput = cv2.warpPerspective(img, matrix, (width,height), cv2.INTER_LINEAR, borderMode=cv2.BORDER_CONSTANT, borderValue=(0,0,0))
        # print(imgOutput.shape)

        # save the warped output
        return imgOutput
    
    def get_chessboard_image(self,img):
        # https://www.pyimagesearch.com/2014/08/25/4-point-opencv-getperspective-transform-example/
        # https://www.geeksforgeeks.org/perspective-transformation-python-opencv/

        # detect edges using gray, then Canny
        img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        canny = cv2.Canny(img_gray, 120,200)
        # retrieve contours by findCountours
        contours, hierarchy = cv2.findContours(canny, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        # img_contour = cv2.drawContours(img, contours, -1, (0,255,75), 1)
        # cv2.imshow('contours',img_contour)
        for con in contours:
            # rec = cv2.boundingRect(con)
            area = cv2.contourArea(con)
            if area > 17000:
                perspectived = self.get_perspective_from_contour(img, con)
                if perspectived is not None:
                    whole_board_image = perspectived[0:self.__CROP_HEIGHT, 0:self.__CROP_WIDTH]
                    cv2.imshow('whole_board', whole_board_image)
                    cv2.waitKey(1)
                    self.__show_detection_line(whole_board_image)
                    return whole_board_image

    def auto_perspect(self, origin_image):
        # Get corners position from detecting aruco marks
        # The sequerence is always [TopLeft, TopRight,bottomRight,BottomLeft]
        corners = self.find_corners(origin_image)
        # print(corners)
        if corners != None:
            if len(corners) >= len(self.__mark_ids):
                # Get perspectived image
                perspect_img = self.get_perspective_view(origin_image,corners)
                if config.publish_mqtt:
                    g_mqtt.publish_cv_image('gobot_stonehouse/eye/perspect', perspect_img)
                return perspect_img
        return None

    def draw_axis(self,img, yaw, pitch, roll, tdx=None, tdy=None, size = 100):

        pitch = pitch * np.pi / 180
        yaw = -(yaw * np.pi / 180)
        roll = roll * np.pi / 180

        if tdx != None and tdy != None:
            tdx = tdx
            tdy = tdy
        else:
            height, width = img.shape[:2]
            tdx = width / 2
            tdy = height / 2

        # X-Axis pointing to right. drawn in red
        x1 = size * (cos(yaw) * cos(roll)) + tdx
        y1 = size * (cos(pitch) * sin(roll) + cos(roll) * sin(pitch) * sin(yaw)) + tdy

        # Y-Axis | drawn in green
        #        v
        x2 = size * (-cos(yaw) * sin(roll)) + tdx
        y2 = size * (cos(pitch) * cos(roll) - sin(pitch) * sin(yaw) * sin(roll)) + tdy

        # Z-Axis (out of the screen) drawn in blue
        x3 = size * (sin(yaw)) + tdx
        y3 = size * (-cos(yaw) * sin(pitch)) + tdy

        cv2.line(img, (int(tdx), int(tdy)), (int(x1),int(y1)),(0,0,255),3)
        cv2.line(img, (int(tdx), int(tdy)), (int(x2),int(y2)),(0,255,0),3)
        cv2.line(img, (int(tdx), int(tdy)), (int(x3),int(y3)),(255,0,0),2)

        return img 

    def draw_axis_2(self, frame, corners):
        dist=np.array(([[-0.58650416 , 0.59103816, -0.00443272 , 0.00357844 ,-0.27203275]]))
        newcameramtx=np.array([[189.076828   ,  0.    ,     361.20126638]
                            ,[  0 ,2.01627296e+04 ,4.52759577e+02]
                            ,[0, 0, 1]])
        mtx=np.array([[398.12724231  , 0.      ,   304.35638757],
                        [  0.       ,  345.38259888, 282.49861858],
                        [  0.,           0.,           1.        ]])

        rvec, tvec, _ = cv2.aruco.estimatePoseSingleMarkers(corners, 0.05, mtx, dist)
        # 估计每个标记的姿态并返回值rvet和tvec ---不同
        # from camera coeficcients
        (rvec-tvec).any() # get rid of that nasty numpy value array error
        i = 0
        cv2.aruco.drawAxis(frame, mtx, dist, rvec[i, :, :], tvec[i, :, :], 0.03)
        cv2.aruco.drawDetectedMarkers(frame, corners)
        return frame


class Commander(GridFinder):
    def __init__(self):
        self.__mark_ids = [125,126]  # [top, bottom]
        self.__CELLS = 5
        self.__LOWEST_SCALE = 0.7
        GridFinder.__init__(self,self.__mark_ids, None)

    def get_command_from_image(self, image):
        '''
        return -1, if found nothing
        return 1..5 if found avaliable cell in black.
        '''
        centers = self.find_corners(image)
        if len(centers)==2:
            
            print('centers',centers)
            top_x, top_y = centers[0]
            bottom_x, bottom_y = centers[1]
            delta_x = (top_x - bottom_x) / 6
            delta_y = (top_y - bottom_y) / 6

            if False:
                # publish a debug image
                color_red = (0,0,255)
                pen_thickness = 3
                cv2.line(image,(top_x,top_y),(bottom_x,bottom_y),color_red, pen_thickness)
                g_mqtt.publish_cv_image('gobot/test/image',image)

            # Get average color
            sum_color = 0
            for index in range(1, self.__CELLS + 1,1):
                # get x,y position of indicator
                x = int(index * delta_x + bottom_x) 
                y = int(index * delta_y + bottom_y) 
                [b,g,r] = image[y, x]
                sum_color += b
                sum_color += g
                sum_color += r

                #print('index=', index,'(x,y)=(',x,y, ')color=', b,g,r)
                #print('cell_position',index, x, y)
            avg_color = sum_color / 5
            
            # Get which cell is black
            for index in range(1, self.__CELLS + 1, 1):
                x = int(index * delta_x + bottom_x)
                y = int(index * delta_y + bottom_y)
                [b,g,r] = image[y,x]
                cell_color = b + g + r
                if cell_color < avg_color * self.__LOWEST_SCALE:
                    return index
        return -1
 
