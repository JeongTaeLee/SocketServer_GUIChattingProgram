using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SharpDX;

namespace JIGAPClientDXGUI
{
    public partial class Transform : Component
    {
        public Vector3 position { get; set; } = new Vector3(0f, 0f, 0f);
        public Vector3 rotation { get; set; } = new Vector3(0f, 0f, 0f);
        public Vector3 scale { get; set; } = new Vector3(1f, 1f, 1f);
        public Vector3 worldPos { get; set; } = new Vector3();
        public Matrix matWorld { get; set; } = new Matrix();

        private Transform parent = null;
        public Transform Parent
        {
            get
            {
                return parent;
            }
            set
            {
                parent = value;

                if (parent != null)
                    parent.gameObject.transform.ChildList.Add(this);
            }
        }

        private List<Transform> ChildList = new List<Transform>();
    }


    public partial class Transform : Component
    {
        public void TransformUpdate()
        {
            matWorld =  Matrix.Scaling(scale) * Matrix.RotationZ(rotation.Z) * Matrix.Translation(position);

            if (Parent != null)
                matWorld = matWorld * Parent.matWorld;

            worldPos = new Vector3(matWorld.M41, matWorld.M42, matWorld.M43);
        }

        public override void Release()
        {
            if (Parent != null)
            {
                Parent.ChildList.Remove(this);
                Parent = null;
            }

            foreach (Transform trans in ChildList)
            {
                trans.Parent = null;
                trans.gameObject.Destroy = true;
            }
            ChildList.Clear();
        }

        public void SetActiveChild(bool inActive)
        {
            foreach(Transform temp in ChildList)
                temp.gameObject.Active = inActive;
        }

    }
}
