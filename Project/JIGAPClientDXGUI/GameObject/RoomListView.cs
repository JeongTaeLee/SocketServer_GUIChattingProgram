using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using JIGAPClientDXGUI.Engine;


namespace JIGAPClientDXGUI
{
    class RoomListView : GameObject
    {
        private RoomButton[] RoomList = null;

        public int Page { get; set; } = 1;
        public int PageSize = 6;
        

        public override void Init()
        {
            transform.position = new Vector3(20f, 75f, 0f);
        }

        public void NextPage()
        {
            if (RoomList == null) return;

            if ((Page * 5) > RoomList.Length && Page != 1) return;

            ++Page;
            LoadPage(Page);
        }
        public void BackPage()
        {
            if (RoomList == null) return;

            if (Page <= 1) return;

            --Page;
            LoadPage(Page);
        }

        public void LoadPage(int page)
        {
            if (RoomList != null)
            {
                for (int i = 0; i < RoomList.Length; ++i)
                    RoomList[i].Active = false;

                
                if (Page == 1)
                {
                    for (int i = 0; i < PageSize; ++i)
                    {
                        if (i < RoomList.Length)
                        {
                            RoomList[i].Active = true;
                            RoomList[i].transform.position = new Vector3(0f, 90 * i, 0f);
                        }
                        else
                            break;
                    }
                }
                else
                {
                    int End = (Page * PageSize);
                    int list = 0;
                    for (int i = End - PageSize; i < End; ++i, ++list)
                    { 
                        if (i < RoomList.Length)
                        {
                            RoomList[i].Active = true;
                            RoomList[i].transform.position = new Vector3(0f, 90 * list, 0f);
                        }
                        else  
                            break;
                    }
                }
            }
        }

        public void UpdateRoomList()
        {
            if (RoomList != null)
            {
                foreach (RoomButton obj in RoomList)
                {
                    obj.transform.SetParent(null);
                    obj.Destroy = true;
                }

                if (RoomList.Length != 0)
                    Array.Clear(RoomList, 0, RoomList.Length - 1);
            }

            string[] RoomNames = null;
            NetworkManager.GetInst().GetRoomList(ref RoomNames);
            RoomList = new RoomButton[RoomNames.Length];
            int i = 0;
            foreach (string str in RoomNames)
            {
                RoomList[i] = ObjectManager.GetInst().AddObject<RoomButton>();
                RoomList[i].transform.SetParent(this);

                RoomList[i].ButtonPos = new Vector3(0f, 90f * i, 0f);
                RoomList[i].RoomName = str;
                RoomList[i].ButtonRange =  new System.Drawing.Rectangle(0, 0, 580, 71); ;
                RoomList[i].Active = false;
                ++i;
            }

            LoadPage(1);
        }
    }
}
