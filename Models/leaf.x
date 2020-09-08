xof 0303txt 0032


template VertexDuplicationIndices { 
 <b8d65549-d7c9-4995-89cf-53a9a8b031e3>
 DWORD nIndices;
 DWORD nOriginalVertices;
 array DWORD indices[nIndices];
}
template XSkinMeshHeader {
 <3cf169ce-ff7c-44ab-93c0-f78f62d172e2>
 WORD nMaxSkinWeightsPerVertex;
 WORD nMaxSkinWeightsPerFace;
 WORD nBones;
}
template SkinWeights {
 <6f0d123b-bad2-4167-a0d0-80224f25fabb>
 STRING transformNodeName;
 DWORD nWeights;
 array DWORD vertexIndices[nWeights];
 array float weights[nWeights];
 Matrix4x4 matrixOffset;
}

    Frame RootFrame {

      FrameTransformMatrix {
        1.000000,0.000000,0.000000,0.000000,
        0.000000,0.000000,1.000000,0.000000,
        0.000000,1.000000,0.000000,0.000000,
        0.000000,0.000000,0.000000,1.000000;;
      }
    Frame Plane_012 {

      FrameTransformMatrix {
        -0.690600,0.363200,0.625500,0.000000,
        -0.717700,-0.236900,-0.654800,0.000000,
        -0.089700,-0.901100,0.424200,0.000000,
        -0.132800,-0.329100,2.670500,1.000000;;
      }
Mesh {
4;
-0.620600; -2.097300; 1.070400;,
1.002500; -0.458400; -0.007900;,
-0.855300; 1.275300; -0.169500;,
-2.478400; -0.363600; 0.908800;;
1;
4; 0, 3, 2, 1;;
  MeshMaterialList {
    1;
    1;
    0;;
  Material Mat1 {
    1.0; 1.0; 1.0; 1.0;;
    1.0;
    1.0; 1.0; 1.0;;
    0.0; 0.0; 0.0;;
  TextureFilename {    "leaves1.tga";  }
  }  // End of Material
    }  //End of MeshMaterialList
  MeshNormals {
4;
    0.247475; 0.349437; 0.903653;,
    0.247475; 0.349437; 0.903653;,
    0.247475; 0.349437; 0.903653;,
    0.247475; 0.349437; 0.903653;;
1;
4; 0, 3, 2, 1;;
}  //End of MeshNormals
MeshTextureCoords {
4;
1.000000;-1.000000;,
0.000000;-1.000000;,
0.000000;-0.000000;,
1.000000;0.000000;;
}  //End of MeshTextureCoords
 }
}
}
