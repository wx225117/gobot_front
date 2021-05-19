#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sys
sys.path.append('/home/pi/pylib')
from terminal_font import TerminalFont
from mqtt_helper import g_mqtt

import cv2
import numpy


from gogame.chessboard import ChessboardLayout, ChessboardCell
from vision.grid_cell import GridCell

class GridPlate():

    def __init__(self, config):
        self.config = config

        self.__detected_layout = ChessboardLayout('Detected layout')
        self.__history = []  # history of layout.
        self.__history_length = 0 
        # self.__diffs = []

        self.__inspect_cell =  ChessboardCell()
        self.__inspect_cell.from_name(config.robot_eye.layout_scanner.inspecting.cell_name)

        self.__FC_GREEN = TerminalFont.Color.Fore.green
        self.__FC_YELLOW = TerminalFont.Color.Fore.yellow
        self.__FC_RESET = TerminalFont.Color.Control.reset


        self.Min_BlackColor = numpy.array([0, 0, 0])  # 要识别黑子颜色的下限
        self.Max_BlackColor = numpy.array([180, 255, 80])  # 要识别黑子的颜色的上限
        self.Min_WhiteColor = numpy.array([0, 0, 200])  # 要识别白子颜色的下限
        self.Max_WhiteColor = numpy.array([180, 100, 255])  # 要识别白子的颜色的上限

    def __predefined_plate_go_game_board(self):
        self.__ROWS = 19
        self.__COLS = 19

        self.__BLANK = self.config.game_rule.cell_color.blank
        self.__BLACK = self.config.game_rule.cell_color.black
        self.__WHITE = self.config.game_rule.cell_color.white

        self.__SPACE_X = self.config.robot_eye.grid_cell.dimension.cell_space_x
        self.__SPACE_Y = self.config.robot_eye.grid_cell.dimension.cell_space_y
        self.__VIEW_RANGE = 1.6

    def __predefined_plate_commander(self):
        self.__ROWS = 5
        self.__COLS = 1

    def __predefined_plate_stonehouse(self):
        self.__ROWS = 60
        self.__COLS = 90

    def __append_to_history(self, layout):
        '''
        return:
            stable_depth, minimum number is 1.
        '''
        while len(self.__history) > self.__history_length:
            del self.__history[0]
        self.__history.append(layout)

        # update stable_depth
        stable_depth = 1
        if len(self.__history) > 2:
            for i in range(len(self.__history)-1):
                diffs = self.__history[-1].compare_with(self.__history[i]) 
                if len(diffs) == 0:
                    # no different
                    stable_depth += 1
        return stable_depth

    def get_cell_image(self, plate_image, row, col):
        x1 = self.__SPACE_X * col 
        y1 = self.__SPACE_Y * row 
        x2 = x1 + int(self.__SPACE_X * self.__VIEW_RANGE)
        y2 = y1 + int(self.__SPACE_Y * self.__VIEW_RANGE)
        cell_img_big = plate_image[y1:y2, x1:x2]

        shrink_size = 6
        x1 = self.__SPACE_X * col + shrink_size
        y1 = self.__SPACE_Y * row + shrink_size
        x2 = x1 + int(self.__SPACE_X * self.__VIEW_RANGE - 2 * shrink_size)
        y2 = y1 + int(self.__SPACE_Y * self.__VIEW_RANGE - 2 * shrink_size)
        cell_img_small = plate_image[y1:y2, x1:x2]

        is_inspected_cell = False
        self.__inspect_cell.from_name(app_config.robot_eye.layout_scanner.inspecting.cell_name)
        if (col == 18 - self.__inspect_cell.col_id) and (18- row == self.__inspect_cell.row_id):
            cv2.imshow('bbbb',cell_img_big)
            cv2.imshow('ssss',cell_img_small)
            is_inspected_cell = True

        return cell_img_big, cell_img_small


    # def start_scan(self, img_board, history_length=3, show_processing_image=True, pause_second=1):
    def scan_layout(self, plate_image, history_length=3, show_processing_image=True, pause_second=1):
        '''
        return A:
            -1,-1: not detected any layout
        return B:
            the lastest layout in history,
            stable_depth
        '''
        self.__history_length = history_length
        detected_layout = ChessboardLayout('Detected layout')

        plate_gray_image = cv2.cvtColor(plate_image, cv2.COLOR_BGR2GRAY)
        plate_brightness = numpy.mean(plate_gray_image)
        # print('board_brightness()= %d' %board_brightness)

        grid_cell = GridCell(plate_brightness)

        # Split board_image to 361 samll images. detect circle one by one
        for col in range(0,self.__COLS):
            for row in range(0,self.__ROWS):
                # crop to small image, around cell center
                cell_img_big, cell_img_small = self.get_cell_image(plate_image, col,row)

                # color = grid_cell.scan(cell_img,is_inspected_cell)
                stone_color = grid_cell.scan_white(cell_img_big, is_inspected=False)
                detected_layout.play_col_row(col_id=18-col, row_id=18-row, color_code=stone_color)
                if stone_color != self.__WHITE:
                    stone_color = grid_cell.scan_black(cell_img_small, is_inspected=False)
                    detected_layout.play_col_row(col_id=18-col, row_id=18-row, color_code=stone_color)

        stable_depth = self.__append_to_history(detected_layout)
        # target_layout.print_out()
        if config.robot_eye.layout_scanner.show_scan_image:
            g_mqtt.publish_cv_image('test',plate_image)


        return self.__history[-1], stable_depth


    def __show_debug(self, img_board,stable_depth):
        copy = img_board.copy()
        cv2.putText(copy, 'Depth= ' + str(stable_depth),(10,50),cv2.FONT_HERSHEY_SIMPLEX,1,(0,255,0),1)
        if len(self.__history) > 2:
            diffs = self.__history[-1].compare_with(self.__history[-2])
            text = ''
            for name,color_a, color_b in diffs:
                text += '' + name + ','
               # draw a red circle on each different cells
                diff_cell = ChessboardCell()
                diff_cell.from_name(name)
                x,y = diff_cell.to_camera__board_xy()
                cv2.circle(copy, (x,y), 16, (0,0,255), 2)
            # draw a blue circle on last_moving cell
            cell = ChessboardCell()
            cell_name = config.current_game.lastest_move_cell_name
            if cell_name is not None:
                cell.from_name(cell_name)
                x,y = cell.to_camera__board_xy()
                cv2.circle(copy, (x,y),16, (255,0,0), 3)
            cv2.putText(copy, 'diffs= ' + text, (10,100), cv2.FONT_HERSHEY_SIMPLEX, 1, (0,255,0), 1)
            cv2.imshow('layout_scanner', copy)
            is_success, img_encode = cv2.imencode(".jpg", copy)
            # if is_success:
                # img_pub = img_encode.tobytes()
                # config.server.mqtt.client.publish(topic='gogame/eye/layout_scanner/chess_board', payload=img_pub)
            cv2.waitKey(1)


if __name__ == "__main__":
    runner = GridCell()