#pragma once

#include <list>
#include <unordered_map>
#include <unordered_set>

#include "Renderable.h"


namespace core
{
	namespace _2d
	{

		class RenderQueue
		{
		public:
			typedef std::vector<const Renderable*> RenderableList;
			typedef std::unordered_map<MaterialId, RenderableList> RenderableListByMaterial;
			typedef std::unordered_map<Priority, RenderableListByMaterial> RenderableGroupMap;
			typedef std::unordered_set<Priority> RenderPrioritySet;

		protected:
			RenderableGroupMap renderableMap;
			RenderPrioritySet usedPrioritySet;
			bool isPriorityListSorted;
			std::list<Priority> sortedPriorityList;
			std::list<Priority>::iterator priorityIterator;


			void sortByPriority()
			{
				sortedPriorityList.insert(sortedPriorityList.end(), usedPrioritySet.begin(), usedPrioritySet.end());
				sortedPriorityList.sort();
				isPriorityListSorted = true;
				priorityIterator = sortedPriorityList.begin();
			};


		public:

			void addRenderable(const Renderable* _rend)
			{
				Priority pr = _rend->getPriority();
				renderableMap[pr][_rend->getMaterialId()].push_back(_rend);
				usedPrioritySet.insert(pr);
			};

			void clear()
			{
				isPriorityListSorted = false;
				priorityIterator = sortedPriorityList.end();
				renderableMap.clear();
				usedPrioritySet.clear();
			};


			void resetPriorityIterator()
			{
				if (!isPriorityListSorted)
					sortByPriority();
				priorityIterator = sortedPriorityList.begin();
			};


			const RenderableListByMaterial* getNextPriorityRenderableList()
			{
				if (!isPriorityListSorted)
					sortByPriority();

				if (priorityIterator == sortedPriorityList.end())
					return nullptr;

				RenderableListByMaterial* list = &(renderableMap[(*priorityIterator)]);
				++priorityIterator;

				return list;
			};





		};
	}
}
