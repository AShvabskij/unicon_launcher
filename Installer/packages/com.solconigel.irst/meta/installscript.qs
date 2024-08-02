function Component() {}

Component.prototype.createOperations = function() {
    component.createOperations();
//  component.addOperation("Copy", "appunicon_launcher.exe", "@TargetDir@/appunicon_launcher.exe");
    // Add additional operations as needed for your files

    // Create desktop shortcut
    component.addOperation("CreateShortcut", "@TargetDir@/appunicon_launcher.exe", "@DesktopDir@/IRST.lnk",
                           "iconPath=@TargetDir@/resources/logo.ico");

    // Create Start Menu shortcut
    component.addOperation("CreateShortcut", "@TargetDir@/appunicon_launcher.exe", "@StartMenuDir@/IRST.lnk",
                           "iconPath=@TargetDir@/resources/logo.ico");
}
