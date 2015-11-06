using UnityEngine;
using System.Collections;

public class Fractal : MonoBehaviour {
	public Mesh mesh;
	public Material material;
	
	public int maxDepth;
	public int depth;
	
	public float childScale;
	public float maxTwist;
	
	private static Vector3[] childDirections = {
		Vector3.up,
		Vector3.right,
		Vector3.left,
		Vector3.forward,
		Vector3.back
	};
	
	private static Quaternion[] childOrientations = {
		Quaternion.identity,
		Quaternion.Euler(0f, 0f, -90f),
		Quaternion.Euler(0f, 0f, 90f),
		Quaternion.Euler (90f, 0f, 0f),
		Quaternion.Euler (-90f, 0f, 0f)
	};
	
	private Material[,] materials;
	
	private void InitialzieMaterials() {
		materials = new Material[maxDepth + 1, 2];
		float t;
		for (int i = 0; i <= maxDepth; ++i) {
			t = i / (maxDepth - 1f);
			t *= t;
			materials[i, 0] = new Material(material);
			materials[i, 0].color = Color.Lerp (Color.white, Color.gray, t);
			materials[i, 1] = new Material(material);
			materials[i, 1].color = Color.Lerp (Color.white, Color.black, t);
		}
		materials[maxDepth, 0].color = Color.white;
		materials [maxDepth, 1].color = Color.black;
	}
	
	public void Start() {
		transform.Rotate (Random.Range (-maxTwist, maxTwist), 0f, 0f);
		if (materials == null) {
			InitialzieMaterials();
		}
		gameObject.AddComponent<MeshFilter> ().mesh = mesh;
		gameObject.AddComponent<MeshRenderer> ().material = materials[depth, Random.Range (0, 2)];
		
		if (depth < maxDepth) {
			StartCoroutine(CreateChildren());
		}
	}
	
	private IEnumerator CreateChildren() {
		for (int i = 0; i < childDirections.Length; ++i) {
			yield return new WaitForSeconds(Random.Range (.1f, .5f));
			new GameObject("Fractal Child").AddComponent<Fractal>().Initialize(this, i);
		}
	}
	
	private void Initialize(Fractal parent, int childIndex) {
		mesh = parent.mesh;
		materials = parent.materials;
		maxDepth = parent.maxDepth;
		depth = parent.depth + 1;
		childScale = parent.childScale;
		maxTwist = parent.maxTwist;
		transform.parent = parent.transform;
		transform.localScale = Vector3.one * childScale;
		transform.localPosition = childDirections[childIndex] * (0.5f + 0.5f * childScale);
		transform.localRotation = childOrientations[childIndex];
	}
}
