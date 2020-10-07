from math import sin, cos, sqrt

from PyQt5 import QtWidgets, QtGui, QtCore
import matplotlib

# Make sure that we are using QT5
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure


class CompassTabWidget(QtWidgets.QTabWidget):
    def __init__(self, telemetry_widget, hc12_widget, aprs_widget):
        super().__init__()

        self.compass_telemetry = telemetry_widget
        self.compass_hc12 = hc12_widget
        self.compass_aprs = aprs_widget

        self.init_ui()
        self.center()

    def init_ui(self):
        self.addTab(self.compass_telemetry, 'Telemetry')
        self.addTab(self.compass_hc12, 'HC12')
        self.addTab(self.compass_aprs, 'APRS')

        self.setWindowTitle('Compass')
        self.setFixedSize(450, 450)

    def activate(self):
        self.show()

    def center(self):
        frame = self.frameGeometry()
        mid = QtWidgets.QDesktopWidget().availableGeometry().center()
        frame.moveCenter(mid)
        self.move(frame.topLeft())


class CompassWidget(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()

        self.lat = 1.0
        self.lon = 1.0
        self.compass = Compass(name=' ')
        self.compass_render = DynamicCompass(name=' ', lat_earth=self.lat, lon_earth=self.lon)
        self.coord_lat = QtWidgets.QLineEdit()
        self.coord_lon = QtWidgets.QLineEdit()
        self.enter = QtWidgets.QPushButton('Enter')
        self.coord_lat.returnPressed.connect(self.receive_coords)
        self.coord_lon.returnPressed.connect(self.receive_coords)
        self.enter.clicked.connect(self.receive_coords)

        self.validator = QtGui.QDoubleValidator(-180.0, 180.0, 7)

        self.layout = QtWidgets.QVBoxLayout()
        self.coord = QtWidgets.QHBoxLayout()

        self.init_ui()

    def init_ui(self):
        self.coord_lat.setValidator(self.validator)
        self.coord_lon.setValidator(self.validator)
        self.coord_lat.setPlaceholderText('Latitude')
        self.coord_lon.setPlaceholderText('Longitude')

        self.layout.addWidget(self.compass_render)
        self.coord.addWidget(QtWidgets.QLabel('Enter your coordinates: '))
        self.coord.addWidget(self.coord_lat)
        self.coord.addWidget(self.coord_lon)
        self.coord.addWidget(self.enter)
        self.layout.addLayout(self.coord)
        self.setLayout(self.layout)

    @QtCore.pyqtSlot()
    def receive_coords(self):
        lat = self.coord_lat.text()
        lon = self.coord_lon.text()

        if lat != '':
            lat = lat.replace(',', '.')
            self.lat = float(lat)
        if lon != '':
            lon = lon.replace(',', '.')
            self.lon = float(lon)
        self.compass_render.change_earth_position(self.lat, self.lon)
        self.coord_lat.clear()
        self.coord_lon.clear()


class Compass(FigureCanvas):
    def __init__(self, name, parent=None, width=3, height=3, dpi=100):
        fig = matplotlib.figure.Figure(figsize=(width, height), dpi=dpi)

        self.axes = fig.add_subplot(111, polar=True)
        self.axes.set_xticklabels(['N', 'NW', 'W', 'SW', 'S', 'SE', 'E', 'NE'])
        self.compute_initial_figure()
        self.axes.set_theta_zero_location('N')
        self.axes.set_ylim(0, 10)

        FigureCanvas.__init__(self, fig)
        self.setParent(parent)
        self.setWindowTitle('Compass')

        FigureCanvas.setSizePolicy(self, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        FigureCanvas.updateGeometry(self)

    def compute_initial_figure(self):
        pass


class DynamicCompass(Compass):
    def __init__(self, name, lat_earth, lon_earth, *args, **kwargs):
        super().__init__(self, *args, **kwargs)
        self.y_label = name
        self.lat_earth = lat_earth
        self.lon_earth = lon_earth
        self.lat_sat = 0.1
        self.lon_sat = 0.1

    def compute_initial_figure(self):
        self.axes.plot((0, 0), (0, 10))

    def change_earth_position(self, lat, lon):
        self.lat_earth = lat
        self.lon_earth = lon

    def update_compass(self, packet):
        if isinstance(packet, dict):
            angle = sqrt((self.lat_sat - self.lat_earth) ** 2 + (self.lon_sat - self.lon_earth) ** 2) / \
                sqrt((self.lon_sat - self.lon_earth) ** 2)

            x = 10 * cos(angle) ** 2
            y = 10 * sin(angle) ** 2

            self.axes.cla()
            self.axes.set_yticks(range(0, 10, 1))
            self.axes.set_xticklabels(['N', 'NW', 'W', 'SW', 'S', 'SE', 'E', 'NE'])
            self.axes.set_theta_zero_location('N')
            self.axes.set_ylim(0, 10)
            self.axes.plot((0, x), (0, y))
            self.draw()
