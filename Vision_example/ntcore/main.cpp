#include <ntcore.h>
#include <iostream>
#include <string>

#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>

int main()
{
	  auto inst = nt::NetworkTableInstance::GetDefault();
	  auto table = inst.GetTable("datatable");
	  auto entry = table->GetEntry("Value");
  
  inst.StartClientTeam(5553);

  while (true)
  {
    if (entry.Exists())
	{
	  std::string value = entry.GetString("not found");
	  
	  if(value == "Hello RaspberryPi !!!")
	  {
		  std::cout << "Le message \"Hello RaspberryPi !!!\" a été reçu" << std::endl << std::endl;
		  std::cout << "Envoi du message \"Hello RoboRIO !!!\" ..." << std::endl << std::endl;
		  entry.SetString("Hello RoboRIO !!!");
	  }
	  else
	  {
		  std::cout << value << std::endl << std::endl;
	  }
    }
  }
  return 0;
}
