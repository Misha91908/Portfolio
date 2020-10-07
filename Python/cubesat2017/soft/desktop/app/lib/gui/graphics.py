import datetime

from PyQt5 import QtWidgets
import matplotlib
# Make sure that we are using QT5
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure


class GraphicsTab(QtWidgets.QTabWidget):

    def __init__(self):
        super().__init__()

        self.temperature_inside_graphic_canvas = MPLCanvas(name="Temperature inside, \u2103")
        self.temperature_inside_graphic = DynamicMPLCanvas(name="Temperature inside, \u2103")
        self.temperature_outside_graphic_canvas = MPLCanvas(name="Temperature outside, \u2103")
        self.temperature_outside_graphic = DynamicMPLCanvas(name="Temperature outside, \u2103")
        self.pressure_graphic_canvas = MPLCanvas(name="Pressure, kPa")
        self.pressure_graphic = DynamicMPLCanvas(name="Pressure, kPa")
        self.ax_graphic_canvas = MPLCanvas(name="Acceleration(OX)")
        self.ax_graphic = DynamicMPLCanvas(name="Acceleration(OX)")
        self.ay_graphic_canvas = MPLCanvas(name="Acceleration(OY)")
        self.ay_graphic = DynamicMPLCanvas(name="Acceleration(OY)")
        self.az_graphic_canvas = MPLCanvas(name="Acceleration(OZ)")
        self.az_graphic = DynamicMPLCanvas(name="Acceleration(OZ)")
        self.voltage_graphic_canvas = MPLCanvas(name="Voltage, V")
        self.voltage_graphic = DynamicMPLCanvas(name="Voltage, V")
        self.current_graphic_canvas = MPLCanvas(name="Current, A")
        self.current_graphic = DynamicMPLCanvas(name="Current, A")

        self.addTab(self.temperature_inside_graphic, 'Temperature inside')
        self.addTab(self.temperature_outside_graphic, 'Temperature outside')
        self.addTab(self.pressure_graphic, 'Pressure')
        self.addTab(self.ax_graphic, 'Acceleration(OX)')
        self.addTab(self.ay_graphic, 'Acceleration(OY)')
        self.addTab(self.az_graphic, 'Acceleration(OZ)')
        self.addTab(self.voltage_graphic, 'Battery Voltage')
        self.addTab(self.current_graphic, 'Battery Current')

    def update_graphics(self, packet):
        if isinstance(packet, dict):
            self.temperature_inside_graphic.update_figure(packet['temp_in'], datetime.datetime.fromtimestamp(packet['time']))
            self.temperature_outside_graphic.update_figure(packet['temp_out'], datetime.datetime.fromtimestamp(packet['time']))
            self.pressure_graphic.update_figure(packet['pressure'], datetime.datetime.fromtimestamp(packet['time']))
            self.ax_graphic.update_figure(packet['axel_x'], datetime.datetime.fromtimestamp(packet['time']))
            self.ay_graphic.update_figure(packet['axel_y'], datetime.datetime.fromtimestamp(packet['time']))
            self.az_graphic.update_figure(packet['axel_z'], datetime.datetime.fromtimestamp(packet['time']))
            self.voltage_graphic.update_figure(packet['bat_volt'], datetime.datetime.fromtimestamp(packet['time']))
            self.current_graphic.update_figure(packet['bat_amp'], datetime.datetime.fromtimestamp(packet['time']))


class MPLCanvas(FigureCanvas):

    def __init__(self, name, parent=None, width=5, height=4, dpi=100):
        fig = matplotlib.figure.Figure(figsize=(width, height), dpi=dpi)

        self.axes = fig.add_subplot(111)
        self.compute_initial_figure()
        self.axes.set_xlabel("x axis")
        self.axes.set_ylabel("y axis")
        self.axes.tick_params(axis='x', rotation=30, labelsize='small', pad=-2)

        FigureCanvas.__init__(self, fig)
        self.setParent(parent)

        FigureCanvas.setSizePolicy(self, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        FigureCanvas.updateGeometry(self)

    def compute_initial_figure(self):
        pass


class DynamicMPLCanvas(MPLCanvas):

    def __init__(self, name, *args, **kwargs):
        super().__init__(self, *args, **kwargs)
        self.x = []
        self.y = []
        self.counter = []
        self.y_label = name
        self.size_buf = 10

    def compute_initial_figure(self):
        self.axes.plot([0], [0])

    def update_figure(self, data, timestamp):
        self.y.append(data)
        self.x.append(timestamp)

        self.axes.set_xticklabels(self.x, fontsize='6', rotation=75)
        if len(self.x) > self.size_buf:
            del self.x[0]
            del self.y[0]

        self.axes.cla()
        self.axes.plot(self.x, self.y)
        self.axes.tick_params(axis='x', rotation=35, labelsize='small', pad=-2)
        self.axes.set_xlabel("Time, hour:minute:second")
        self.axes.set_ylabel(self.y_label)
        self.draw()
