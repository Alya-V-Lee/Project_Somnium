


#include "UI/Widget/MainUserWidget.h"

void UMainUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}

