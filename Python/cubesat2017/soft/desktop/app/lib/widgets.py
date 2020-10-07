from lib.base import BaseContentWidget

from PyQt5 import QtCore, QtGui, QtWidgets


class TelemetryContentWidget(BaseContentWidget):
    disconnection_signal = QtCore.pyqtSignal()

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.disconnection_case_notification = QtWidgets.QWidget()
        self.init_disconnection_notification()
        self.producer.disconnection = self.disconnection_sig
        self.consumer.disconnection = self.disconnection_sig
        self.producer.bug_tracker_signal = self.bug_tracker.update_bug_tracker_signal
        self.disconnection_signal.connect(self.disconnection_case)

    def init_disconnection_notification(self):
        self.disconnection_case_notification.setWindowTitle('Lost connection!')
        self.disconnection_case_notification.setFixedSize(500, 150)
        label = QtWidgets.QLabel('Telemetry port device is not found! \n '
                                 'Please, check a wire connection or plug in your device.',
                                 self.disconnection_case_notification)
        label.setAlignment(QtCore.Qt.AlignCenter)
        label.move(65, 60)

        frame = self.disconnection_case_notification.frameGeometry()
        mid = QtWidgets.QDesktopWidget().availableGeometry().center()
        frame.moveCenter(mid)
        self.disconnection_case_notification.move(frame.topLeft())

    def disconnection_sig(self):
        self.disconnection_signal.emit()

    def disconnection_case(self):
        self.disconnection_case_notification.show()


class HC12TelemetryContentWidget(BaseContentWidget):
    disconnection_signal = QtCore.pyqtSignal()

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.disconnection_case_notification = QtWidgets.QWidget()
        self.init_disconnection_notification()
        self.producer.disconnection = self.disconnection_sig
        self.consumer.disconnection = self.disconnection_sig
        self.producer.bug_tracker_signal = self.bug_tracker.update_bug_tracker_signal
        self.disconnection_signal.connect(self.disconnection_case)

    def init_disconnection_notification(self):
        self.disconnection_case_notification.setWindowTitle('Lost connection!')
        self.disconnection_case_notification.setFixedSize(500, 150)
        label = QtWidgets.QLabel('HC12 port device is not found! \n'
                                 ' Please, check a wire connection or plug in your device.',
                                 self.disconnection_case_notification)
        label.setAlignment(QtCore.Qt.AlignCenter)
        label.move(65, 60)

        frame = self.disconnection_case_notification.frameGeometry()
        mid = QtWidgets.QDesktopWidget().availableGeometry().center()
        frame.moveCenter(mid)
        self.disconnection_case_notification.move(frame.topLeft())

    def disconnection_sig(self):
        self.disconnection_signal.emit()

    def disconnection_case(self):
        self.disconnection_case_notification.show()


class APRSContentWidget(BaseContentWidget):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.producer.bug_tracker_signal = self.bug_tracker.update_bug_tracker_signal
