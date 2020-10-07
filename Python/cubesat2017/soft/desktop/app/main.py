import logging
import sys
import os
from queue import Queue

from PyQt5 import QtWidgets, QtGui, QtCore

from lib.gui.compas import CompassTabWidget
from lib.gui.bug_tracker import BugTrackerTabWidget
from lib.threads import (
    TelemetryProducerThread,
    TelemetryConsumerThread,
    HC12TelemetryProducerThread,
    HC12TelemetryConsumerThread,
    APRSProducerThread,
    APRSConsumerThread
)
from lib.widgets import TelemetryContentWidget, HC12TelemetryContentWidget, APRSContentWidget


packet_queue_telemetry = Queue()
packet_queue_hc12 = Queue()
packet_queue_aprs = Queue()

telemetry_keyword = 'telemetry'
aprs_keyword = 'APRS'
hc12_keyword = 'HC12_telemetry'


class App(QtWidgets.QMainWindow):
    update_status_signal_telemetry = QtCore.pyqtSignal(bool)
    update_status_signal_hc12 = QtCore.pyqtSignal(bool)
    start_signal = QtCore.pyqtSignal()
    stop_signal = QtCore.pyqtSignal()

    def __init__(self):
        super().__init__()

        self.update_status_signal_telemetry.connect(self.update_status_telemetry)
        self.update_status_signal_hc12.connect(self.update_status_hc12)
        self.content_main = QtWidgets.QTabWidget()

        self.session_telemetry_status = False
        self.session_HC12_status = False

        self.bug_tracker = BugTrackerTabWidget('Changelog')

        self.producer_thread_telemetry = TelemetryProducerThread(
            status=self.session_telemetry_status,
            status_updater=self.update_status_sig_telemetry,
        )
        self.producer_thread_hc12 = HC12TelemetryProducerThread(
            status=self.session_HC12_status,
            status_updater=self.update_status_sig_hc12,
        )
        self.producer_thread_aprs = APRSProducerThread(
            status=self.session_telemetry_status,
            status_updater=self.update_status_sig_telemetry,
        )
        self.consumer_thread_telemetry = TelemetryConsumerThread()
        self.consumer_thread_hc12 = HC12TelemetryConsumerThread()
        self.consumer_thread_aprs = APRSConsumerThread()

        self.logger_telemetry = logging.getLogger(telemetry_keyword)
        self.logger_telemetry.setLevel(logging.INFO)

        self.logger_hc12 = logging.getLogger(hc12_keyword)
        self.logger_hc12.setLevel(logging.INFO)

        self.logger_aprs = logging.getLogger(aprs_keyword)
        self.logger_aprs.setLevel(logging.INFO)

        self.content_telemetry = TelemetryContentWidget(
            producer_thread=self.producer_thread_telemetry,
            consumer_thread=self.consumer_thread_telemetry,
            queue=packet_queue_telemetry,
            logger=self.logger_telemetry,
            bug_tracker=self.bug_tracker.bug_tracker_telemetry,
            keyword=telemetry_keyword
        )
        self.content_hc12 = HC12TelemetryContentWidget(
            producer_thread=self.producer_thread_hc12,
            consumer_thread=self.consumer_thread_hc12,
            queue=packet_queue_hc12,
            logger=self.logger_hc12,
            bug_tracker=self.bug_tracker.bug_tracker_hc12,
            keyword=hc12_keyword
        )
        self.content_aprs = APRSContentWidget(
            producer_thread=self.producer_thread_aprs,
            consumer_thread=self.consumer_thread_aprs,
            queue=packet_queue_aprs,
            logger=self.logger_aprs,
            bug_tracker=self.bug_tracker.bug_tracker_aprs,
            keyword=aprs_keyword
        )

        self.compass_main = CompassTabWidget(self.content_telemetry.compass, self.content_hc12.compass,
                                             self.content_aprs.compass)

        self.start_signal.connect(self.start)
        self.stop_signal.connect(self.stop)

        self.setCentralWidget(self.content_main)
        self.init_ui()
        self.center()
        self.setWindowTitle('SaTTY v. 1.4.0')

        start_action = QtWidgets.QAction(QtGui.QIcon('data/img/start.png'), 'Run', self)
        start_action.setShortcut('Ctrl+R')
        start_action.setStatusTip('Run Receive')
        start_action.triggered.connect(self.start)

        stop_action = QtWidgets.QAction(QtGui.QIcon('data/img/stop.png'), 'Stop', self)
        stop_action.setShortcut('Ctrl+T')
        stop_action.setStatusTip('Stop Receive')
        stop_action.triggered.connect(self.stop)

        compass = QtWidgets.QAction(QtGui.QIcon('data/img/compass.png'), 'Compass', self)
        compass.setShortcut('Ctrl+P')
        compass.setStatusTip('Show Compass')
        compass.triggered.connect(self.compass_main.activate)

        changelog = QtWidgets.QAction(QtGui.QIcon('data/img/bug.png'), 'Bug tracker', self)
        changelog.setShortcut('Ctrl+B')
        changelog.setStatusTip('Show Changelog')
        changelog.triggered.connect(self.bug_tracker.activate)

        self.statusBar = QtWidgets.QStatusBar()
        self.session_status_telemetry_message = QtWidgets.QLabel('Current session status Telemetry: ' +
                                                                 str(self.session_telemetry_status))
        self.session_status_HC12_message = QtWidgets.QLabel('Current session status HC12: ' +
                                                            str(self.session_HC12_status))

        self.statusBar.addPermanentWidget(self.session_status_telemetry_message)
        self.statusBar.addPermanentWidget(self.session_status_HC12_message)
        self.setStatusBar(self.statusBar)

        tools = self.addToolBar('Toolbar')
        tools.addAction(start_action)
        tools.addAction(stop_action)
        tools.addAction(compass)
        tools.addAction(changelog)

    def init_ui(self):
        self.content_main.addTab(self.content_telemetry, 'Telemetry')
        self.content_main.addTab(self.content_hc12, 'HC12')
        self.content_main.addTab(self.content_aprs, 'APRS')

        self.resize(1450, 1050)

    def update_status_sig_telemetry(self, status):
        self.update_status_signal_telemetry.emit(status)

    def update_status_sig_hc12(self, status):
        self.update_status_signal_hc12.emit(status)

    @QtCore.pyqtSlot(bool)
    def update_status_telemetry(self, status):
        self.session_status_telemetry_message.setText('Current session status Telemetry: ' + str(status))

    @QtCore.pyqtSlot(bool)
    def update_status_hc12(self, status):
        self.session_status_HC12_message.setText('Current session status HC12: ' + str(status))

    def start(self):
        self.producer_thread_telemetry.wake_up()
        self.producer_thread_hc12.wake_up()
        self.producer_thread_aprs.wake_up()
        self.consumer_thread_telemetry.wake_up()
        self.consumer_thread_hc12.wake_up()
        self.consumer_thread_aprs.wake_up()
        self.producer_thread_telemetry.status_updater(True)

    def stop(self):
        self.producer_thread_telemetry.sleep_()
        self.producer_thread_hc12.sleep_()
        self.producer_thread_aprs.sleep_()
        self.consumer_thread_telemetry.sleep_()
        self.consumer_thread_hc12.sleep_()
        self.consumer_thread_aprs.sleep_()
        self.producer_thread_telemetry.status_updater(False)

    def center(self):
        frame = self.frameGeometry()
        mid = QtWidgets.QDesktopWidget().availableGeometry().center()
        frame.moveCenter(mid)
        self.move(frame.topLeft())


def log_dir_init():
    if not os.path.isdir('log/'):
        os.mkdir('log/')


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    log_dir_init()
    start = App()
    start.show()
    sys.exit(app.exec_())
