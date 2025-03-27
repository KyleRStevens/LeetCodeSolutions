#pragma once

// Standard
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>
#include <mutex>
#include <set>
#include <stack>
#include <unordered_set>
#include <vector>

class Solution_RabbitHole
{
private:
	int m_maxVisitableWebpages = 1;

private:
	struct Page
	{
		bool visited = false;
		//int bestPath = -1; // TODO
		std::set<int> fromLinks; // O(N) space
		std::set<int> maxVisitablePages; // O(N) space
	};

public:
	bool UpdateMaxVisitablePages(std::vector<Page>& pages, int page, int updaterPage)
	{
		// Check if we actually need to do an update...
		bool maxVisitablePagesUpdated = false;
		if (pages[updaterPage].maxVisitablePages.size() > pages[page].maxVisitablePages.size())
		{
			// Set equal to the updater's set
			pages[page].maxVisitablePages = pages[updaterPage].maxVisitablePages;

			// Add back in the self-reference
			pages[page].maxVisitablePages.insert(page);

			// Mark the path of this best visit path
			//pages[page].bestPath = updaterPage;

			// Flag the update
			maxVisitablePagesUpdated = true;
		}
		else if (pages[updaterPage].maxVisitablePages.size() == pages[page].maxVisitablePages.size())
		{
			// If it's the same size, but does NOT constain THIS page, then it will be bigger, so lets do the update
			if (pages[updaterPage].maxVisitablePages.find(page) == pages[updaterPage].maxVisitablePages.end())
			{
				// Set equal to the updater's set
				pages[page].maxVisitablePages = pages[updaterPage].maxVisitablePages;

				// Add back in the self-reference
				pages[page].maxVisitablePages.insert(page);

				// Mark the path of this best visit path
				//pages[page].bestPath = updaterPage;

				// Flag the update
				maxVisitablePagesUpdated = true;
			}
		}

		// Update the global max visitable pages size if necessary
		if (maxVisitablePagesUpdated)
		{
			m_maxVisitableWebpages = std::max(m_maxVisitableWebpages, static_cast<int>(pages[page].maxVisitablePages.size()));
		}

		// Return if there was an update or not
		return maxVisitablePagesUpdated;
	}

	int getMaxVisitableWebpages(int N, std::vector<int>& L) // ~O(N* L)
	{
		// Create a container for the page data
		std::vector<Page> pages;
		pages.resize(N + 1); // O(N)

		// Build out the whole graph
		for (int i = 0; i < L.size(); ++i) // O(L) => ~O(N*L)
		{
			int fromPage = i + 1;
			int toPage = L[i];

			// Add the "from" link in the "to" page
			pages[toPage].fromLinks.insert(fromPage); // At least O(logN)

			// Update the from page's maxVisitablePages based on the toPage?
			//bool maxVisitablePagesUpdated = UpdateMaxVisitablePages(pages, fromPage, toPage);
		}

		// An array of "visited" flags (for each page)
		//std::vector<bool> visited;
		//visited.resize(N + 1, false); // O(N)

		// A queue for the "recursive" updating
		std::queue<int> pagesToUpdate;

		// For each page...
		for (int i = 1; i < pages.size(); ++i) // O(N) ==> Somewhere between O(N) & O(N^3)?
		{
			// TODO
			// Rather than starting at this page, traverse the graph as far as you can using either the saved best path if possible or else any random path (starting at the end of a path reduces revisits)
			//std::set<int> tempVisited;
			//int tempPage = i;
			//while (pages[tempPage].bestPath != -1 && !tempVisited.contains(bestPath))
			//{
			//
			//}
			//while (fromLinks != empty)
			//{
			//
			//}

			// If the page is NOT flagged as visited...
			if (pages[i].visited == false)
			{
				// Add the current start page to the queue
				pagesToUpdate.push(i);
				
				// Recursively update each "from" page's maxVisitablePages based on the current page
				while (pagesToUpdate.empty() == false) // O(N) ==> O(N^2) worst case, O(N+) average?
				{
					// Get the current page and remove from the queue
					int currentPage = pagesToUpdate.front();
					pagesToUpdate.pop();

					// If the page isn't already visited...
					if (pages[currentPage].visited == false)
					{
						// Visit the page
						pages[currentPage].visited = true;

						// For each of its "from" links...
						for (const int& page : pages[currentPage].fromLinks) // O(N) in the worst case, but for average case, this (sort of / hopefully) doesn't count as additional time complexity because visited pages will be skipped (mostly), so the total number of true iterations is still ~N (where there's hopefully not many revisits)
						{
							// See if any of them get updated with this page's maxVisitablePages
							if (UpdateMaxVisitablePages(pages, page, currentPage))
							{
								// Only if the page has already been visited do we queue it to be visited again - if it hasn't been visited, it already will be! No need to keep queuing it redundent times as who knows how many other nodes will also try to do the same...
								if (pages[page].visited == true)
								{
									// If so, it might need to update on of its "from" pages, so add it to the queue
									pagesToUpdate.push(page);

									// Mark it as not visited even if it has been so we can re-update its connections if necessary
									pages[page].visited = false;
								}
							}
						}
					}
				}
			}
		}

		// Return
		return m_maxVisitableWebpages;
	}
};