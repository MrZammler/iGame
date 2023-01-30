/*
  slavesList.c
  slaves list functions source for iGame

  Copyright (c) 2018-2022, Emmanuel Vasilakis

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
#include "slavesList.h"

slavesList *slavesListHead = NULL;
slavesList *slavesListBuffer = NULL;

int isListEmpty(void *head)
{
	return head == NULL;
}

void slavesListAddHead(slavesList *node)
{
	if (node != NULL)
	{
		node->next = NULL;

		if (isListEmpty(slavesListHead))
		{
			slavesListHead = node;
		}
		else
		{
			node->next = slavesListHead;
			slavesListHead = node;
		}
	}
}

void slavesListAddTail(slavesList *node)
{
	if (node != NULL)
	{
		node->next = NULL;

		if (isListEmpty(slavesListHead))
		{
			slavesListHead = node;
		}
		else
		{
			// find the last node
			slavesList *currPtr = slavesListHead;
			while (currPtr->next != NULL)
			{
				currPtr = currPtr->next;
			}

			currPtr->next = node;
		}
	}
}

void slavesListPrint(void)
{
	size_t cnt = 0;
	slavesList *currPtr = slavesListHead;
	while (currPtr != NULL)
	{
		// printf("----> %d\t%s\t%s\n", currPtr->instance, currPtr->title, currPtr->genre);
		// printf("----> %s\t%d\t%d\n", currPtr->path, currPtr->favourite, currPtr->times_played);
		// printf("----> %d\t%d\t%s\n\n", currPtr->last_played, currPtr->hidden, currPtr->user_title);
		currPtr = currPtr->next;
		cnt++;
	}
	printf("END OF LIST: %d items\n", cnt);
}

/*
*	Search the slavesList by item path.
*	If found returns the node, otherwise returns NULL
*
*	TODO: When we have the list sorted make this binary search
*/
slavesList *slavesListSearchByPath(char *path, unsigned int pathSize)
{
	if (isListEmpty(slavesListHead))
	{
		return NULL;
	}

	slavesList *currPtr = slavesListHead;
	while (currPtr != NULL && strncmp(currPtr->path, path, pathSize))
	{
		currPtr = currPtr->next;
	}

	return currPtr;
}

slavesList *slavesListSearchByTitle(char *title, unsigned int titleSize)
{
	if (isListEmpty(slavesListHead))
	{
		return NULL;
	}

	slavesList *currPtr = slavesListHead;
	while (
		currPtr != NULL &&
		strncmp(currPtr->user_title, title, titleSize) &&
		strncmp(currPtr->title, title, titleSize)
	) {
		currPtr = currPtr->next;
	}

	return currPtr;
}

void slavesListCountInstancesByTitle(slavesList *node)
{
	if (isListEmpty(slavesListHead))
	{
		return;
	}

	slavesList *currPtr = slavesListHead;
	node->instance = 0;
	while (currPtr != NULL)
	{
		if(
			currPtr->title[0] == node->title[0] &&
			!strncmp(currPtr->title, node->title, sizeof(currPtr->title)) &&
			strncmp(currPtr->path, node->path, sizeof(currPtr->path))
		) {
			node->instance++;
		}
		currPtr = currPtr->next;
	}
}

// void slavesListClearTitles(void)
// {
// 	if (isListEmpty(slavesListHead))
// 	{
// 		return;
// 	}

// 	slavesList *currPtr = slavesListHead;
// 	while (currPtr != NULL)
// 	{
// 		currPtr->title[0] = '\0';
// 		currPtr = currPtr->next;
// 	}
// }

slavesList *getSlavesListHead(void)
{
	return slavesListHead;
}

void setSlavesListBuffer(slavesList *node)
{
	slavesListBuffer = node;
}

slavesList *getSlavesListBuffer(void)
{
	return slavesListBuffer;
}
