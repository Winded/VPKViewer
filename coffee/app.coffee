ConfigManager = require("./js/lib/ConfigManager")
MenuBar = require("./js/lib/MenuBar")

global.document = window.document
angular = require("angular")

app = angular.module "vpkviewer", [require("ui-router")]

app.service "menuBar", require("./js/lib/MenuBar")

app.directive "fileDialog", require("./js/lib/FileDialog")

app.controller "MainController", require("./js/lib/MainController")
app.controller "ConfigsController", require("./js/lib/ConfigsController")

app.config ($stateProvider, $urlRouterProvider) ->
    $urlRouterProvider.otherwise("/main")
    $stateProvider.state "main",
        url: "/main"
        templateUrl: "templates/main.html"
        controller: "MainController"
    .state "configs",
        url: "/configs",
        templateUrl: "templates/configs.html"
        controller: "ConfigsController"

app.run ($state, menuBar) ->

    menuBar.initialize()

    configManager = ConfigManager.getInstance()
    success = configManager.load()
    if not success
        $state.go("configs")