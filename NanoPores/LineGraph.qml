import QtQuick 2.5
import QtCharts 2.0
import DataSource 1.0

LineSeries {
    id: lineSeriesRoot
    property DataSource dataSource

    function updateData() {
        clear()
        append(0,0)
        append(1,1)
    }

    onDataSourceChanged: {
        dataSource.updated.connect(updateData)
        updateData()
    }
}
