// qrc:/main.qml
import QtQuick 2.0
import QtQuick.Window 2.0

Window {
    id: win
    visible: true
    width: 960
    height: 600
    title: "Telemetry (Canvas)"

    // данные из C++
    property var points: []     // [{x,y}, ...]
    property real minX: 0
    property real maxX: 1
    property real minY: 0
    property real maxY: 1

    // простая "кнопка" для перерисовки (без Controls)
    Rectangle {
        id: redraw
        x: 12; y: 12; width: 140; height: 36; radius: 6
        color: "#2e7dd1"
        Text { anchors.centerIn: parent; text: "Перерисовать"; color: "white" }
        MouseArea { anchors.fill: parent; onClicked: { core.loadTelemetry("");canvas.requestPaint(); } }
    }

    Canvas {
        id: canvas
        anchors {
            top: redraw.bottom; topMargin: 8
            left: parent.left; right: parent.right; bottom: parent.bottom
        }
        antialiasing: true

        onPaint: {
            const ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)

            if (!points || points.length < 2) return

            // поля
            const padL = 50, padR = 10, padT = 10, padB = 35
            const w = width  - padL - padR
            const h = height - padT - padB

            const dx = (maxX - minX) || 1
            const dy = (maxY - minY) || 1
            const sx = x => padL + (x - minX) * w / dx
            const sy = y => padT + h - (y - minY) * h / dy

            // оси
            ctx.strokeStyle = "#808080"
            ctx.lineWidth = 1
            ctx.beginPath()
            ctx.moveTo(padL, padT); ctx.lineTo(padL, padT + h)
            ctx.lineTo(padL + w, padT + h)
            ctx.stroke()

            // подписи по осям (по минимуму)
            ctx.fillStyle = "#666"
            ctx.font = "12px sans-serif"
            ctx.fillText(minX.toFixed(2), padL, padT + h + 20)
            ctx.fillText(maxX.toFixed(2), padL + w - 40, padT + h + 20)
            ctx.save()
            ctx.translate(10, padT + 12)
            ctx.rotate(-Math.PI / 2)
            ctx.fillText("value", 0, 0)
            ctx.restore()
            ctx.fillText("t, s", padL + w/2 - 15, padT + h + 28)

            // линия графика
            ctx.strokeStyle = "#2e7dd1"
            ctx.lineWidth = 1.5
            ctx.beginPath()
            ctx.moveTo(sx(points[0].x), sy(points[0].y))
            for (let i = 1; i < points.length; ++i) {
                const p = points[i]
                ctx.lineTo(sx(p.x), sy(p.y))
            }
            ctx.stroke()
        }
    }

    // получаем точки из Core::telemetryLoaded(QVariantList)
    Connections {
        target: core
        function onTelemetryLoaded(list) {
            points = list || []
            if (points.length === 0) { canvas.requestPaint(); return }

            // авто-диапазоны
            minX = maxX = points[0].x
            minY = maxY = points[0].y
            for (let i = 1; i < points.length; ++i) {
                const p = points[i]
                if (p.x < minX) minX = p.x
                if (p.x > maxX) maxX = p.x
                if (p.y < minY) minY = p.y
                if (p.y > maxY) maxY = p.y
            }
            canvas.requestPaint()
        }
    }
}
