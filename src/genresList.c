/*
  genresList.c
  genres list functions source for iGame

  Copyright (c) 2018-2025, Emmanuel Vasilakis

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
#include "genresList.h"
#include "strfuncs.h"

genresList *genresListHead = NULL;

static int isListEmpty(const void *head)
{
	return head == NULL;
}

void genresListAddTail(genresList *node)
{
	if (node != NULL)
	{
		node->next = NULL;

		if (isListEmpty(genresListHead))
		{
			genresListHead = node;
		}
		else
		{
			// find the last node
			genresList *currPtr = genresListHead;
			while (currPtr->next != NULL)
			{
				currPtr = currPtr->next;
			}

			currPtr->next = node;
		}
	}
}

static BOOL genresListRemoveHead(void) {

	if (isListEmpty(genresListHead)) {
		return FALSE;
	}

	genresList *nextPtr = genresListHead->next;
	free(genresListHead);
	genresListHead = nextPtr;
	return TRUE;
}

void genresListPrint(void)
{
	int cnt = 0;
	genresList *currPtr = genresListHead;
	while (currPtr != NULL)
	{
		printf("----> %s\n", currPtr->title);
		currPtr = currPtr->next;
		cnt++;
	}
	printf("END OF LIST: %d items\n", cnt);
}

genresList *genresListSearchByTitle(const char *title, unsigned int titleSize)
{
	if (isListEmpty(genresListHead))
	{
		return NULL;
	}

	genresList *currPtr = genresListHead;
	while (
		currPtr != NULL &&
		strncmp(currPtr->title, title, titleSize)
	) {
		currPtr = currPtr->next;
	}

	return currPtr;
}

genresList *getGenresListHead(void)
{
	return genresListHead;
}

void emptyGenresList(void)
{
	while(genresListRemoveHead())
	{}
}

int genresListNodeCount(int cnt)
{
	static int nodeCount = 0;

	if (cnt == -1)
	{
		return nodeCount;
	}

	nodeCount = cnt;
	return nodeCount;
}

genresList *addGenreInList(const char *title)
{
	if (isStringEmpty(title))
		return NULL;

	if (genresListSearchByTitle(title, sizeof(char) * MAX_GENRE_NAME_SIZE) == NULL)
	{
		genresList *node = malloc(sizeof(genresList));
		if(node == NULL)
		{
			return NULL;
		}
		strncpy(node->title, title, sizeof(char) * MAX_GENRE_NAME_SIZE);
		genresListAddTail(node);
		return node;
	}
	return NULL;
}

void sortGenresList(void)
{
	if (isListEmpty(genresListHead) || genresListHead->next == NULL) {
		// Empty list or single node - already sorted
		return;
	}

	genresList *sorted = NULL;
	genresList *current = genresListHead;

	// Remove each node from original list and insert into sorted list
	while (current != NULL) {
		// Save next node before we detach current
		genresList *next = current->next;

		// Insert current node into sorted list in correct position
		if (sorted == NULL || strncmp(current->title, sorted->title, MAX_GENRE_NAME_SIZE) <= 0) {
			// Insert at beginning of sorted list
			current->next = sorted;
			sorted = current;
		} else {
			// Find insertion point in sorted list
			genresList *search = sorted;
			while (search->next != NULL &&
				   strncmp(current->title, search->next->title, MAX_GENRE_NAME_SIZE) > 0) {
				search = search->next;
			}
			// Insert after search node
			current->next = search->next;
			search->next = current;
		}

		// Move to next node in original list
		current = next;
	}

	// Update head pointer
	genresListHead = sorted;
}
