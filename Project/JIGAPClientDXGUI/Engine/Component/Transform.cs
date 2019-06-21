using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SharpDX;

namespace JIGAPClientDXGUI.Engine
{
    public class Transform : Component
    {
        public Vector3 position { get; set; } = new Vector3(0f, 0f, 0f);
        public Vector3 rotation { get; set; } = new Vector3(0f, 0f, 0f);
        public Vector3 scale { get; set; } = new Vector3(1f, 1f, 1f);
        public Matrix matWorld { get; set; } = new Matrix();
        public Vector3 worldPos { get; set; } = new Vector3();

        public GameObject parent { get; private set; } = null;

        public void TransformUpdate()
        {
            matWorld =  Matrix.Scaling(scale) * Matrix.RotationZ(rotation.Z) * Matrix.Translation(position);

            if (parent != null)
                matWorld *= parent.transform.matWorld;

            worldPos = new Vector3(matWorld.M41, matWorld.M42, matWorld.M43);
        }

        public void SetParent(GameObject inParent)
        {
            parent = inParent;
        }
    }
}
