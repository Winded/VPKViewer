# Load bootstrap
global.jQuery = require("jquery")
require("bootstrap")

ipc = require("ipc")

angular = require("angular")

app = angular.module "vpkviewer", [require("ui-router")]

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

app.run ($state) ->
    ipc.on "goto-view", (arg) ->
        $state.go(arg.state, if arg.params? then arg.params else null)