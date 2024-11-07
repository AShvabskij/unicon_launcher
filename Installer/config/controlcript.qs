function Controller()
{
    console.log("controller");
    installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
    installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
    installer.setDefaultPageVisible(QInstaller.LicenseCheck, false);
    installer.setDefaultPageVisible(QInstaller.StartMenuSelection, true);

}

Controller.prototype.IntroductionPageCallback = function()
{

    var widget = gui.currentPageWidget(); // get the current wizard page
    if (widget != null) {
//        widget.title = "New title."; // set the page title
        widget.MessageLabel.setText("Welcome to the RST Setup"); // set the welcome text
    }
}
