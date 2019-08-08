using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;

namespace JIGAPClientDXGUI
{
    class ListView : Component
    {
        List<GameObject> ElementList = new List<GameObject>();

        public int PageMaxElementSize { get; set; } = 5;
        public float ElementDistance { get; set; } = 50f;
        public int NowPage { get; private set; } = 0;

        public override void Release()
        {
            foreach( var element in ElementList)
                element.Destroy = true;
        }

        public void NextPage()
        {
            if ((ElementList.Count / PageMaxElementSize) <= NowPage)
                return;

            UnDisplayPage(NowPage);
            ++NowPage;
            DisplayPage(NowPage);

        }
        public void RevertPage()
        {
            if (NowPage <= 0)
                return;

            UnDisplayPage(NowPage);
            --NowPage;
            DisplayPage(NowPage);
        }

        private void DisplayPage(int inPage = 0)
        {
            if (ElementList.Count == 0)
                return;

            Vector3 position = new Vector3();

            int count = 0;
            for (int i = inPage * PageMaxElementSize; i < (inPage + 1) * PageMaxElementSize; ++i, ++count)
            {
                if (i + 1 > ElementList.Count)
                    return;

                if (count == 0)
                    position = new Vector3(0f, 0f, 0f);
                else
                    position = new Vector3(0f, (ElementDistance * count), 0f);

                ElementList[i].Active = true;
                ElementList[i].transform.position = position;
            }

        }
        private void UnDisplayPage(int inPage = 0)
        {
            if (ElementList.Count == 0)
                return;

            for (int i = inPage * PageMaxElementSize; i < (inPage + 1) * PageMaxElementSize; ++i)
            {
                if (i + 1 > ElementList.Count)
                    return;

                ElementList[i].Active = false;
            }

        }

        public void ClearListObject()
        {
            foreach (var element in ElementList)
                element.Destroy = true;
            ElementList.Clear();
        }
        public void AddListObject(GameObject inObject)
        {
            if (inObject == null)
                return;
            inObject.transform.Parent = transform;
            inObject.Active = false;

            ElementList.Add(inObject);
        }
        
        public void SettingList()
        {
            DisplayPage();
        }
    }
}
