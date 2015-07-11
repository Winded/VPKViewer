gui = window.require("nw.gui")
VPKLoader = require("./VPKLoader")

module.exports =
class MenuBar
    @$inject = ["$state"]

    constructor: (@state) ->
        @menu = new gui.Menu(type: "menubar")

        @fileMenu = new gui.Menu()
        @fileMenu.append new gui.MenuItem
            label: "Open"
            key: "o"
            modifiers: "ctrl"
            click: @openFile

        @viewMenu = new gui.Menu()
        @viewMenu.append new gui.MenuItem
            label: "Configs"
            click: @openConfigsView

        @menu.append new gui.MenuItem(label: "File", submenu: @fileMenu)
        @menu.append new gui.MenuItem(label: "View", submenu: @viewMenu)

    initialize: () ->
        gui.Window.get().menu = @menu

    openFile: () =>
        windowManager = WindowManager.getInstance()
        vpkLoader = VPKLoader.getInstance()
        fileArr = Dialog.showOpenDialog windowManager,
            title: "Open VPK file"
            filters: [
                name: "VPK files", extensions: ["vpk"]
            ]
            properties: ["openFile"]
        if fileArr? and fileArr.length >= 1
            file = fileArr[0]
            vpkLoader.load(file)
            windowManager = WindowManager.getInstance()
            windowManager.mainWindow.webContents.send("goto-view", state: "main")

    openConfigsView: () =>
        @state.go("configs")