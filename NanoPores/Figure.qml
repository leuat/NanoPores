import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtCharts 2.0
import DataSource 1.0
ChartView {
    id: chartViewRoot

    property alias axisX: _axisX
    property alias axisY: _axisY

    antialiasing: true
    legend.visible: false

    ValueAxis {
        id: _axisX
        tickCount: 5
        min: 0
        max: 1
    }

    ValueAxis {
        id: _axisY
        tickCount: 5
        min: 0
        max: 1
    }

}
