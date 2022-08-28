#include "gbafe.h"
#include "event/event.h"
#include "random/hash.h"

int EvCheck07_CHES(EventInfo* info){
	ChestEventDefinition* chestEvent = (ChestEventDefinition*)info->triggerScript;
	if(info->xPos != chestEvent->xPos || info->yPos != chestEvent->yPos){
		return false;
	}

	info->script = (void*)1;
	info->eId = chestEvent->eId;
	info->commandId = chestEvent->commandId;
	info->unk_10 = chestEvent->unk_06; // ??
	info->unk_14 = HashByte_N((info->xPos + info->yPos), NOISE_CHEST, 0xBB); // item from the chest
	return true;
}
