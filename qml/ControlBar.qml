import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

Rectangle {
    id: actionPanel

    property int gridX : 10
    property int gridY : 10

    color: "red"
    radius:10

    Column {
        leftPadding: 10
        topPadding:10
        spacing: 10

        /********************** Set Grid *********************/
        Text {
            text: "SET GRID"
            font.pixelSize: 15
            color: "black"
        }

        TextField {
            width: 100
            placeholderText: "Iterations"
            id: nbIterations
            validator: RegExpValidator { regExp: /[0-9]+/ }
        }

        Row {
            TextField {
                width: 50
                id: gridX
                text: actionPanel.gridX
                validator: RegExpValidator { regExp: /[0-9]+/ }
            }

            TextField {
                width: 50
                id: gridY
                text: actionPanel.gridY
                validator: RegExpValidator { regExp: /[0-9]+/ }
            }
        }

        Button {
            text: "Redessiner la grille"
            onClicked : {
                actionPanel.gridX = gridX.text;
                actionPanel.gridY = gridY.text;

                Context.resize(Number(gridY.text), Number(gridX.text));
            }
        }


        /********************** Controls *********************/
        Text {
            text: "CONTROLS"
            font.pixelSize: 15
            color: "black"
        }

        Button {
            text: "Rand"
            onClicked : {
                Context.rand();
            }
        }

        Row {
            Button {
                text: "Reset"
                onClicked : {
                    Context.reset();
                }
            }

            Button {
                text: "Start"
                onClicked : {
                    var iterationsMax = Number(nbIterations.text);

                    if (iterationsMax <= 0)
                    {
                        Context.play();
                    }
                    else
                    {
                        Context.play(iterationsMax);
                    }
                }
            }
        }

        Row {
            Button {
                text: "Backward"
                onClicked : {
                    Context.back();
                }
            }

            Button {
                text: "Forward"
                onClicked : {
                    Context.stepByStep();
                }
            }
        }


        /********************** Data *********************/
        Text {
            text: "DATA"
            font.pixelSize: 15
            color: "black"
        }

        Row {
            Button {
                text: "Save"

                onClicked : {
                    fileSaveDialog.visible = true
                }

                FileDialog {
                    id: fileSaveDialog
                    title: "Please choose a file"
                    folder: shortcuts.home
                    selectExisting: false

                    onAccepted: {
                        Context.save(fileSaveDialog.fileUrl);
                        Qt.quit();
                    }

                    onRejected: {
                        console.log("Canceled")
                        Qt.quit()
                    }

                    Component.onCompleted: visible = false
                }
            }

            Button {
                text: "Load"
                onClicked : {
                    fileOpenDialog.visible = true
                }

                FileDialog {
                    id: fileOpenDialog
                    title: "Please choose a file"
                    folder: shortcuts.home

                    onAccepted: {
                        Context.load(fileOpenDialog.fileUrl);
                        Qt.quit();
                    }

                    onRejected: {
                        console.log("Canceled")
                        Qt.quit()
                    }

                    Component.onCompleted: visible = false
                }
            }
        }

        /********************** Speed *********************/
        Text {
            text: "SPEED"
            font.pixelSize: 15
            color: "black"
        }

        Slider {
            id: slideTemporisation
            value: 1
            width: 140

            onValueChanged: {
                Context.setTemporisation(value);
            }
        }
    }
}
