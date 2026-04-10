<div align="center">
  <h1>📊 SortRace: MFC Parallel Benchmarking</h1>
  <p><strong>A Windows-Native GUI for Multithreaded vs. Sequential Algorithm Analysis</strong></p>
  
  <img src="https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" />
  <img src="https://img.shields.io/badge/Framework-MFC-red?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Threading-Multi--Core-orange?style=for-the-badge" />
</div>

<hr />

<h3>🖥️ Application Overview</h3>
<p>
  <b>SortRace</b> is an MFC-based desktop application developed to visualize and measure the efficiency gap between <b>Sequential</b> and <b>Multithreaded</b> sorting algorithms. Unlike standard console benchmarks, this tool provides a native Windows interface for real-time performance monitoring.
</p>

<h3>🛠️ Core Implementation</h3>
<ul>
  <li><b>GUI Engine:</b> Built using the <code>CWinApp</code> and <code>CDialogEx</code> classes within the Microsoft Foundation Class (MFC) framework.</li>
  <li><b>Concurrency Logic:</b> Compares standard linear sorting against parallelized versions that leverage modern multi-core processor architectures.</li>
  <li><b>Visual Themes:</b> Implements <code>CMFCVisualManagerWindows</code> to ensure a native, modern Windows look and feel for all UI controls.</li>
  <li><b>Memory Management:</b> High-performance execution with strict lifecycle management via <code>CShellManager</code> and <code>InitInstance</code> overrides.</li>
</ul>

<h3>🚀 Performance Comparison Features</h3>
<table style="width:100%">
  <tr>
    <th>Feature</th>
    <th>Sequential Execution</th>
    <th>Multithreaded Execution</th>
  </tr>
  <tr>
    <td><b>Resource Usage</b></td>
    <td>Single-thread block</td>
    <td>Parallel task distribution</td>
  </tr>
  <tr>
    <td><b>Visual Feedback</b></td>
    <td>Linear progress updates</td>
    <td>Concurrent thread activity tracking</td>
  </tr>
  <tr>
    <td><b>Analysis</b></td>
    <td>Baseline O(n log n) efficiency</td>
    <td>Speedup ratio (S = T_seq / T_par)</td>
  </tr>
</table>

<h3>⚙️ Build Instructions</h3>
<p><b>Prerequisites:</b> Visual Studio 2022 with the <i>"C++ MFC for latest v143 build tools"</i> component installed.</p>

<p><b>1. Load & Build:</b></p>
<pre><code>// Open SortRace.sln
// Build -> Rebuild Solution (Release | x64)</code></pre>

<p><b>2. Run the Race:</b></p>
<p>The application launches in <b>Modal Dialog</b> mode. Select your dataset size, choose between Sequential or Multithreaded modes, and trigger the "Race" to see the timing results.</p>

<hr />
