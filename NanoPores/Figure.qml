import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtCharts 2.0
import DataSource 1.0
ChartView {
    id: chartViewRoot

    property alias axisX: _axisX
    property alias axisY: _axisY
    property var lineGraphs: []

    antialiasing: true
    legend.visible: false

    function updateLimits() {
        var xmin = 1e10
        var xmax = -1e10
        var ymin = 1e10
        var ymax = -1e10
        for(var i in chartViewRoot.lineGraphs) {
            var lineGraph = chartViewRoot.lineGraphs[i]
            xmin = Math.min(lineGraph.dataSource.xMin(), xmin)
            xmax = Math.max(lineGraph.dataSource.xMax(), xmax)
            ymin = Math.min(lineGraph.dataSource.yMin(), ymin)
            ymax = Math.max(lineGraph.dataSource.yMax(), ymax)
        }

        _axisX.min = xmin*0.9
        _axisX.max = xmax*1.1
        _axisY.min = ymin*0.9
        _axisY.max = ymax*1.1
    }

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
