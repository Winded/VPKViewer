Dialog = require("dialog")
Menu = require("menu")
ipc = require("ipc")
WindowManager = require("./WindowManager")
VPKLoader = require("./VPKLoader")

module.exports =
class MenuBar
    @instance: null
    @getInstance: () ->
        @instance = new MenuBar() if not @instance?
        return @instance

    constructor: () ->
        @menus = [
            {
                label: "File"
                submenu: [
                    {
                        label: "Open"
                        accelerator: "Ctrl+O"
                        click: @openFile
                    }
                ]
            }
            {
                label: "View"
                submenu: [
                    {
                        label: "Configs"
                        click: @openConfigsView
                    }
                    {
                        label: "Reload"
                        accelerator: "Ctrl+R"
                        click: () -> WindowManager.getInstance().mainWindow.reload()
                    }
                ]
            }
        ]

    initialize: () ->
        @menu = Menu.buildFromTemplate(@menus)
        Menu.setApplicationMenu(@menu)

    openFile: () ->
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

    openConfigsView: () ->
        windowManager = WindowManager.getInstance()
        windowManager.mainWindow.webContents.send("goto-view", state: "configs")