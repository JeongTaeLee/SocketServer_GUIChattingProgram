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
        private RoomButton[] roomButtons = null;

        public int Page { get; set; } = 1;
        public int PageSize = 6;
        

        public override void Init()
        {
            transform.position = new Vector3(20f, 75f, 0f);
        }

        public void NextPage()
        {
            if (roomButtons == null) return;

            if ((Page * PageSize) > roomButtons.Length && Page != 1) return;

            LoadPage(Page + 1);
        }
        public void BackPage()
        {
            if (roomButtons == null) return;

            if (Page <= 1) return;

            LoadPage(Page - 1);
        }

        public void LoadPage(int _page)
        {
            if (roomButtons.Length == 0) return;

            if (roomButtons != null)
            {
                // 전 페이지 비활성화.
                for (int i = (Page * PageSize) - PageSize; i < (Page * PageSize);  ++i)
                {
                    roomButtons[i].Active = false;

                    if (roomButtons.Length - 1 == i )
                        break;
                }

                int posCount = 0;
                Page = _page;
                // 현재 페이지 활성화.
                for (int i = (Page * PageSize) - PageSize; i < (Page * PageSize); ++i, ++posCount)
                {
                    roomButtons[i].Active = true;
                    roomButtons[i].transform.position = new SharpDX.Vector3(0f, 90 * posCount, 0f);

                    if (roomButtons.Length - 1 == i)
                        break;
                }
            }
        }

        public void UpdateRoomList()
        {

            if (roomButtons != null)
            {
                for (int i = 0; i < roomButtons.Length; ++i)
                {
                    roomButtons[i].transform.SetParent(null);
                    roomButtons[i].Destroy = true;
                }

                if (roomButtons.Length > 0)
                    Array.Clear(roomButtons, 0, roomButtons.Length);
            }

            string[] RoomNames = null;
            NetworkManager.GetInst().GetRoomList(ref RoomNames);
            if (RoomNames.Length == 0) return;


            roomButtons = new RoomButton[RoomNames.Length];
            for (int i = 0; i < roomButtons.Length; ++i)
            {
                roomButtons[i] = ObjectManager.GetInst().AddObject<RoomButton>();
                roomButtons[i].transform.SetParent(this);
                roomButtons[i].RoomName = RoomNames[i];
                roomButtons[i].Active = false;
            }


            LoadPage(1);
        }
    }
}
