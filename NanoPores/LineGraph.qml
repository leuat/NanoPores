import QtQuick 2.5
import QtCharts 2.0
import DataSource 1.0

LineSeries {
    id: lineSeriesRoot
    property DataSource dataSource
    property Figure figure

    function updateData() {
        clear()
        if(dataSource === undefined) {
            return;
        }
        for(var i in dataSource.xValues) {
            var x = dataSource.xValues[i]
            var y = dataSource.yValues[i]
            append(x,y)
        }
    }

    onDataSourceChanged: {
        dataSource.updated.connect(updateData)
        updateData()
    }
}
