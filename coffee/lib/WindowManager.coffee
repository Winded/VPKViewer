BrowserWindow = require("browser-window")

module.exports =
class WindowManager
    @instance: null
    @getInstance: () ->
        @instance = new WindowManager() if not @instance?
        return @instance

    constructor: () ->
        @mainWindow = null

    initialize: () ->
        @mainWindow = new BrowserWindow
            width: 800
            height: 600
            "min-width": 400
            "min-height": 300
            show: false
        @mainWindow.loadUrl("file://#{__dirname}/../../index.html")
        @mainWindow.openDevTools()
        @mainWindow.on "closed", () =>
            @mainWindow = null

        return true