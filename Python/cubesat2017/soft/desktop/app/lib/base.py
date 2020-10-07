import logging
import datetime

from PyQt5 import QtWidgets, QtCore, QtGui

from lib.gui import logs, graphics, compas
import lib.exeptions

telemetry_log_schema = [
    'packet_id',
    'sats',
    'time',
    'temp_in',
    'temp_out',
    'pressure',
    'axel_x',
    'axel_y',
    'axel_z',
    'lat',
    'lon',
    'alt',
    'hdop',
    'vdop',
    'bat_volt',
    'bat_amp',
]

aprs_log_schema = [
    'packet_id',
    'sats',
    'time',
    'temp_in',
    'temp_out',
    'pressure',
    'lat',
    'lon',
    'alt',
    'bat_volt',
    'bat_amp'
]


class BaseContentWidget(QtWidgets.QWidget):
    init_logger_signal = QtCore.pyqtSignal()
    clear_handler_signal = QtCore.pyqtSignal()

    def __init__(self,
                 producer_thread,
                 consumer_thread,
                 queue,
                 logger,
                 bug_tracker,
                 keyword
                 ):
        super().__init__()

        self.log = logs.LogWindow()
        self.log.update_signal.connect(self.update_log_window)
        self.cursor_status = True
        self.keyword = keyword

        self.bug_tracker = bug_tracker
        self.bug_tracker.update_bug_tracker_signal.connect(self.update_bug_tracker)
        self.init_logger_signal.connect(self.init_bug_tracker_logger)
        self.clear_handler_signal.connect(self.clear_log_bug_tracker_handler)

        self.graphics = graphics.GraphicsTab()

        self.compass = compas.CompassWidget()

        self.thread_queue = queue

        self.producer = producer_thread
        self.producer.queue = self.thread_queue

        self.consumer = consumer_thread
        self.consumer.queue = self.thread_queue
        self.consumer.tasks = (
            self.log.update_logger,
            self.graphics.update_graphics,
            self.log_data,
            self.compass.compass_render.update_compass
        )

        self.logger = logger
        self.init_logger_signal.connect(self.init_data_logger)
        self.clear_handler_signal.connect(self.clear_log_data_handler)
        self.producer.logger_signal = self.init_logger_signal
        self.producer.clear_handler = self.clear_handler_signal

        self.clear_button = QtWidgets.QPushButton('Clear Text')
        self.clear_button.clicked.connect(self.clear_logger)

        self.cursor_status_checkbox = QtWidgets.QCheckBox('Scroll Down')
        self.cursor_status_checkbox.setCheckState(QtCore.Qt.Checked)
        self.cursor_status_checkbox.toggled.connect(self.scrolling_logs)

        self.copy_text = QtWidgets.QPushButton('Copy Text In Buffer')
        self.copy_text.clicked.connect(self.copy_data_to_clipboard)

        self.button_layout = QtWidgets.QHBoxLayout()
        self.log_layout = QtWidgets.QVBoxLayout()
        self.layout = QtWidgets.QGridLayout()

        self.init_ui()

    def init_ui(self):
        self.button_layout.addWidget(self.clear_button)
        self.button_layout.addWidget(self.cursor_status_checkbox)
        self.button_layout.addWidget(self.copy_text)
        self.button_layout.setAlignment(QtCore.Qt.AlignCenter)

        self.log_layout.addWidget(self.log)
        self.log_layout.addLayout(self.button_layout)

        self.layout.addLayout(self.log_layout, 1, 1)
        self.layout.addWidget(self.graphics, 1, 2)
        self.setLayout(self.layout)

    def start_receive(self):
        self.start_signal.emit()

    def stop_receive(self):
        self.stop_signal.emit()

    def clear_logger(self):
        self.log.clear()

    def scrolling_logs(self):
        self.cursor_status = False if self.cursor_status is True else True

    def copy_data_to_clipboard(self):
        self.log.selectAll()
        self.log.copy()
        cursor = self.log.textCursor()
        cursor.setPosition(cursor.selectionEnd(), QtGui.QTextCursor.MoveAnchor)
        self.log.setTextCursor(cursor)

    @staticmethod
    def get_current_data():
        return datetime.datetime.strftime(datetime.datetime.now(), "%Y.%m.%d %H:%M:%S")

    @QtCore.pyqtSlot()
    def init_data_logger(self):
        handler = logging.FileHandler('log/log_' + self.keyword + '_' + self.get_current_data() + '.csv', mode='w')
        handler.setLevel(logging.INFO)
        self.logger.addHandler(handler)

    @QtCore.pyqtSlot()
    def init_bug_tracker_logger(self):
        handler = logging.FileHandler('log/log_bug_tracker_' + self.get_current_data() + '.log', mode='w')
        handler.setLevel(logging.INFO)
        self.bug_tracker.logger.addHandler(handler)

    @QtCore.pyqtSlot()
    def clear_log_data_handler(self):
        for handler in self.logger.handlers[:]:
            self.logger.removeHandler(handler)

    @QtCore.pyqtSlot()
    def clear_log_bug_tracker_handler(self):
        for handler in self.bug_tracker.logger.handlers[:]:
            self.bug_tracker.logger.removeHandler(handler)

    @QtCore.pyqtSlot(dict, type)
    def update_log_window(self, packet, error):
        if error is lib.exeptions.NoExeptions:
            self.log.insertPlainText(logs.template_telemetry.format(**packet))
            if self.cursor_status:
                cursor = self.log.textCursor()
                cursor.setPosition(cursor.selectionEnd(), QtGui.QTextCursor.MoveAnchor)
                self.log.setTextCursor(cursor)
        else:
            self.log.insertPlainText('========================================================================='
                                     '========\n' + 'Packet is not valid! ' + str(error) + '\n')
            if self.cursor_status:
                cursor = self.log.textCursor()
                cursor.setPosition(cursor.selectionEnd(), QtGui.QTextCursor.MoveAnchor)
                self.log.setTextCursor(cursor)

    @QtCore.pyqtSlot(object)
    def update_bug_tracker(self, data):
        self.bug_tracker.insertPlainText(str(data) + '\n')
        self.bug_tracker.logger.info(str(data))

    def log_data(self, packet):
        self.bug_tracker.update_bug_tracker_signal.emit('Data for logging:' + str(packet))
        if isinstance(packet, dict):
            if self.keyword is 'telemetry':
                schema = telemetry_log_schema
            else:
                schema = aprs_log_schema

            sorted_list_of_data = []
            for key in schema:
                sorted_list_of_data.append(str(packet[key]))

            self.logger.info(','.join(sorted_list_of_data))
            self.bug_tracker.update_bug_tracker_signal.emit('Successful logging data!')


class BaseThread(QtCore.QThread):
    def __init__(self, queue=None):
        super().__init__()
        self.is_running = False
        self.queue = queue

    def run(self):
        pass

    def wake_up(self):
        if not self.isRunning():
            self.start()

    def sleep_(self):
        self.is_running = False
