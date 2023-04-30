import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.0

Rectangle {
    width:520
    height:420

    Row {
        spacing:20
        x: 20

        ControlBar {
            id: controlBar
            width:200
            height:400
        }

        PlateauVie {
            id: plateauVie
            nbCellX : controlBar.gridX
            nbCellY : controlBar.gridY
            coteCell : 10
        }
    }
}
