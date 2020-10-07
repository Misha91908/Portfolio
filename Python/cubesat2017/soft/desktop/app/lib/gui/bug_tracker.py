import logging
from PyQt5 import QtWidgets, QtCore


class BugTrackerTabWidget(QtWidgets.QTabWidget):
    def __init__(self, keyword):
        super().__init__()

        self.bug_tracker_telemetry = BugTrackerWidget(keyword)
        self.bug_tracker_hc12 = BugTrackerWidget(keyword)
        self.bug_tracker_aprs = BugTrackerWidget(keyword)

        self.init_ui()
        self.center()

    def init_ui(self):
        self.addTab(self.bug_tracker_telemetry, 'Telemetry')
        self.addTab(self.bug_tracker_hc12, 'HC12')
        self.addTab(self.bug_tracker_aprs, 'APRS')

        self.setWindowTitle('Changelog')
        self.setFixedSize(1100, 600)

    def activate(self):
        self.show()

    def center(self):
        frame = self.frameGeometry()
        mid = QtWidgets.QDesktopWidget().availableGeometry().center()
        frame.moveCenter(mid)
        self.move(frame.topLeft())


class BugTrackerWidget(QtWidgets.QTextEdit):
    update_bug_tracker_signal = QtCore.pyqtSignal(object)

    def __init__(self, keyword):
        super().__init__()

        self.logger = logging.getLogger(keyword)
        self.logger.setLevel(logging.INFO)

        self.init_ui()

    def init_ui(self):
        self.setTextInteractionFlags(QtCore.Qt.NoTextInteraction)
        self.setLineWrapMode(QtWidgets.QTextEdit.NoWrap)

    def send_data(self, data):
        self.update_bug_tracker_signal.emit(data)
