import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
Row {
    id: rootItem
    property string name
    property alias string: string.text
    property color textColor: "black"
    property real labelWidth: 80
    spacing: 5
    height: 20
    width: parent.width

    Label {
        text: name+' '
        color: rootItem.textColor
        width: rootItem.labelWidth
    }
    TextField {
        id: string
        width: rootItem.labelWidth
    }
}
