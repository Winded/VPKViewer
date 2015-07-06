Dialog = require("dialog")
Menu = require("menu")
ipc = require("ipc")
WindowManager = require("./WindowManager")

module.exports =
class MenuBar
    @instance = null
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
        # TODO

    openConfigsView: () ->
        windowManager = WindowManager.getInstance()
        windowManager.mainWindow.webContents.send("goto-view", state: "configs")