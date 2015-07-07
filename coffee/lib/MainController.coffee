remote = require("remote")
ConfigManager = remote.require("./lib/ConfigManager")
VPKLoader = remote.require("./lib/VPKLoader")

module.exports =
class MainController
    @$inject = ["$scope"]

    constructor: (@scope) ->
        configManager = ConfigManager.getInstance()
        @scope.configs = configManager.configs
        @scope.selectedConfig = configManager.selectedConfig

        @scope.path = ""
        @scope.files = @getFiles()
        alert(@scope.files.length)

        @scope.configSelected = @configSelected

    configSelected: () =>
        configManager = ConfigManager.getInstance()
        cfg = @scope.selectedConfig
        configManager.selectConfig(cfg.id)
        configManager.save()

    getFiles: () =>
        vpkLoader = VPKLoader.getInstance()
        folders = vpkLoader.foldersInDir(@scope.path)
        files = vpkLoader.filesInDir(@scope.path)
        f = []
        for folder in folders
            f.push
                path: folder.path
                name: vpkLoader.getName(folder.path)
        for file in files
            f.push
                path: file.path
                name: vpkLoader.getName(file.path)