/*
  chipsetList.c
  chipset list functions source for iGame

  Copyright (c) 2018-2023, Emmanuel Vasilakis

  This file is part of iGame.

  iGame is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  iGame is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with iGame. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <exec/types.h>

#include "iGameExtern.h"
#include "chipsetList.h"
#include "strfuncs.h"

chipsetList *chipsetListHead = NULL;

static int isListEmpty(const void *head)
{
	return head == NULL;
}

void chipsetListAddTail(chipsetList *node)
{
	if (node != NULL)
	{
		node->next = NULL;

		if (isListEmpty(chipsetListHead))
		{
			chipsetListHead = node;
		}
		else
		{
			// find the last node
			chipsetList *currPtr = chipsetListHead;
			while (currPtr->next != NULL)
			{
				currPtr = currPtr->next;
			}

			currPtr->next = node;
		}
	}
}

static BOOL chipsetListRemoveHead(void) {

	if (isListEmpty(chipsetListHead)) {
		return FALSE;
	}

	chipsetList *nextPtr = chipsetListHead->next;
	free(chipsetListHead);
	chipsetListHead = nextPtr;
	return TRUE;
}

void chipsetListPrint(void)
{
	int cnt = 0;
	chipsetList *currPtr = chipsetListHead;
	while (currPtr != NULL)
	{
		printf("----> %s\n", currPtr->title);
		currPtr = currPtr->next;
		cnt++;
	}
	printf("END OF LIST: %d items\n", cnt);
}

chipsetList *chipsetListSearchByTitle(char *title, unsigned int titleSize)
{
	if (isListEmpty(chipsetListHead))
	{
		return NULL;
	}

	chipsetList *currPtr = chipsetListHead;
	while (
		currPtr != NULL &&
		strncmp(currPtr->title, title, titleSize)
	) {
		currPtr = currPtr->next;
	}

	return currPtr;
}

chipsetList *getChipsetListHead(void)
{
	return chipsetListHead;
}

void emptyChipsetList(void)
{
	while(chipsetListRemoveHead())
	{}
}

int chipsetListNodeCount(int cnt)
{
	static int nodeCount = 0;

	if (cnt == -1)
	{
		return nodeCount;
	}

	nodeCount = cnt;
	return nodeCount;
}

void addChipsetInList(char *title)
{
	if (isStringEmpty(title))
		return;

	if (chipsetListSearchByTitle(title, sizeof(char) * MAX_CHIPSET_SIZE) == NULL)
	{
		chipsetList *node = malloc(sizeof(chipsetList));
		if(node == NULL)
		{
			return;
		}
		strncpy(node->title, title, sizeof(char) * MAX_CHIPSET_SIZE);
		chipsetListAddTail(node);
	}
}
