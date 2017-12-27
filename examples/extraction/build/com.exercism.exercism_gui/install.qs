
function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
    component.createOperations();
    component.addOperation("CreateDesktopEntry", 
                           "@HomeDir@/.local/share/applications/Exercism.desktop", 
                           "Type=Application\n
			    Terminal=false\n
			    Exec=@TargetDir@/AppWrapper\n
                            Name=Exercism\n
			    Path=@TargetDir@\n
                            Icon=@TargetDir@/logo.png");
    component.addOperation("Copy", 
			   "@HomeDir@/.local/share/applications/Exercism.desktop", 
			   "@HomeDir@/Desktop/Exercism.desktop"
    );

}
