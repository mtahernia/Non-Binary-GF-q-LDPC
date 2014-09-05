<map version="0.9.0">
<!-- To view this file, download free mind mapping software FreeMind from http://freemind.sourceforge.net -->
<node COLOR="#000000" CREATED="1409638411138" ID="ID_1726228935" MODIFIED="1409641878350" TEXT="Execution sequence">
<font NAME="SansSerif" SIZE="12"/>
<node CREATED="1409640228592" FOLDED="true" ID="ID_1905294142" MODIFIED="1409716310403" POSITION="right" TEXT="Get Input arguements from input">
<font NAME="SansSerif" SIZE="12"/>
<node CREATED="1409640255004" ID="ID_368535862" MODIFIED="1409641147177" TEXT="iterations">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409640264100" ID="ID_96172482" MODIFIED="1409641147177" TEXT="inputfilename">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409640270988" ID="ID_652399738" MODIFIED="1409641147177" TEXT="BlockLength">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409640277980" ID="ID_352138514" MODIFIED="1409641147177" TEXT="count_runs">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409640285680" ID="ID_781260334" MODIFIED="1409641147177" TEXT="ChannelType">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409640318988" ID="ID_1954985084" MODIFIED="1409641147177" TEXT="OutputFile">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409640346800" ID="ID_1143337776" MODIFIED="1409641147176" TEXT="Seed">
<font NAME="SansSerif" SIZE="12"/>
</node>
</node>
<node CREATED="1409638424480" ID="ID_1071844953" MODIFIED="1409641148849" POSITION="right" TEXT="Open output file">
<richcontent TYPE="NOTE"><html>
  <head>
    
  </head>
  <body>
    <p>
      ReportBuf.OpenFile(OutputLogFileName);
    </p>
  </body>
</html></richcontent>
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409640427863" ID="ID_1638641049" MODIFIED="1409641148849" POSITION="right" TEXT="Initialize Seed">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409640461695" ID="ID_1564564232" MODIFIED="1409641148849" POSITION="right" TEXT="Open Definition FIle">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409638441401" FOLDED="true" ID="ID_1189177112" MODIFIED="1409716312931" POSITION="right" TEXT="Create LDPC_Code Code from definition file and BlockLength">
<font NAME="SansSerif" SIZE="12"/>
<node CREATED="1409713454801" ID="ID_1946364355" MODIFIED="1409713522824" TEXT="Set Block Length"/>
<node CREATED="1409713523640" ID="ID_979399261" MODIFIED="1409713573329" TEXT="LDPC_Code::GetFromFile">
<node CREATED="1409713576139" ID="ID_1310065297" MODIFIED="1409714699224" TEXT="rho_degs, rho_wts"/>
<node CREATED="1409714685935" ID="ID_1634249420" MODIFIED="1409714687932" TEXT="lambda_degs, lambda_wts "/>
<node CREATED="1409714758682" ID="ID_1940587605" MODIFIED="1409714791349" TEXT="MapInUse.GetFromFile(file)">
<node CREATED="1409714851814" ID="ID_58836200" MODIFIED="1409714871767" TEXT="MapInUse.q &lt;-- file"/>
<node CREATED="1409714793358" ID="ID_1151214198" MODIFIED="1409714882719" TEXT="MapInUse.vals &lt;-- file"/>
</node>
<node CREATED="1409714700707" ID="ID_804087397" MODIFIED="1409714928104" TEXT="MapInUse.GetQ() --&gt; GFq::q"/>
</node>
</node>
<node CREATED="1409638469125" FOLDED="true" ID="ID_1762884976" MODIFIED="1409716316831" POSITION="right" TEXT="Create Channel">
<font NAME="SansSerif" SIZE="12"/>
<node CREATED="1409715562444" ID="ID_575169700" MODIFIED="1409715588986" TEXT="Channel = new AWGN_Channel"/>
<node CREATED="1409715971061" ID="ID_1545884558" MODIFIED="1409715993126" TEXT="Channel.SetNoiseSigma(noise_sigma)"/>
</node>
<node CREATED="1409638478005" FOLDED="true" ID="ID_19250453" MODIFIED="1409716318691" POSITION="right" TEXT="Code.SetChannel(*Channel)">
<font NAME="SansSerif" SIZE="12"/>
<node CREATED="1409716075079" ID="ID_859795458" MODIFIED="1409716115465" TEXT="Channel --&gt; Code.Channel"/>
</node>
<node CREATED="1409638505401" FOLDED="true" ID="ID_694243494" MODIFIED="1409716319331" POSITION="right" TEXT="Channel-&gt;ProcessMapping(Code)">
<font NAME="SansSerif" SIZE="12"/>
<node CREATED="1409716173014" ID="ID_785220020" MODIFIED="1409716193364" TEXT="Code.MapInUse.Normalize()">
<node CREATED="1409716194944" ID="ID_695900780" MODIFIED="1409716259088" TEXT="Normalize Code.Map.vals to have a power of 1"/>
</node>
</node>
<node CREATED="1409638550704" ID="ID_101428037" MODIFIED="1409641148850" POSITION="right" TEXT="Print Channel Data">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409638870710" ID="ID_1767232481" MODIFIED="1409839159409" POSITION="right" TEXT="Code.ResetGrapgh()">
<font NAME="SansSerif" SIZE="12"/>
<node CREATED="1409737184958" ID="ID_658129683" MODIFIED="1409839165791" TEXT="Code.Graph.Reset(BlockLength, lambda_degs, lambda_wts, rho_degs, rho_wts, MapInUse)">
<node CREATED="1409739308292" FOLDED="true" ID="ID_525258313" MODIFIED="1409838272940" TEXT="Clear()">
<node CREATED="1409739318932" ID="ID_296213944" MODIFIED="1409739337321" TEXT="delete variable_nodes"/>
<node CREATED="1409739337772" ID="ID_778458636" MODIFIED="1409739344713" TEXT="delete check_nodes"/>
<node CREATED="1409739345028" ID="ID_1143626073" MODIFIED="1409739354713" TEXT="delete edges"/>
<node CREATED="1409739356820" ID="ID_156939020" MODIFIED="1409739363809" TEXT="delete EdgeStack"/>
</node>
<node CREATED="1409740000559" FOLDED="true" ID="ID_1155715655" MODIFIED="1409838272212" TEXT="Calculate parameters">
<node CREATED="1409739372308" ID="ID_1774552626" MODIFIED="1409739484984" TEXT="Calculate Ratio=M/N=\frac{\sum\rho_i/i}{\sum\lambda_j/j}"/>
<node CREATED="1409739485899" ID="ID_998731944" MODIFIED="1409739513164" TEXT="N = Block length"/>
<node CREATED="1409739514623" ID="ID_1717638499" MODIFIED="1409739551863" TEXT="M = (int) ceil (ratio*N)"/>
<node CREATED="1409739553151" ID="ID_1857642677" MODIFIED="1409739596471" TEXT="E = (int) floor(N / \sum\lambda_j/j);"/>
</node>
<node CREATED="1409739954972" FOLDED="true" ID="ID_1713172958" MODIFIED="1409838271468" TEXT="Generate nodes and edges">
<node CREATED="1409739605622" ID="ID_523893888" MODIFIED="1409739853449" TEXT="edges = new edge[E]"/>
<node CREATED="1409739858444" ID="ID_506456160" MODIFIED="1409739882505" TEXT="variable_nodes = new variable_node[N]"/>
<node CREATED="1409739883260" ID="ID_631993653" MODIFIED="1409739905125" TEXT="check_nodes = new check_node[M]"/>
<node CREATED="1409739907256" ID="ID_704320825" MODIFIED="1409739942656" TEXT="EdgeStack = new edge*[E*2]"/>
</node>
<node CREATED="1409827815116" FOLDED="true" ID="ID_782592006" MODIFIED="1409838270714" TEXT="Generate Sockets">
<node CREATED="1409828690520" ID="ID_269435030" MODIFIED="1409828696067" TEXT="variable_node **left_sockets = new variable_node*[E];"/>
<node CREATED="1409828697180" ID="ID_1135651317" MODIFIED="1409828711452" TEXT="check_node **right_sockets = new check_node*[E];"/>
<node CREATED="1409828712400" ID="ID_1409184111" MODIFIED="1409828727448" TEXT="edge **EdgeStackPointer = EdgeStack;         // Auxiliary pointer to stack"/>
</node>
<node CREATED="1409834835273" FOLDED="true" ID="ID_924262990" MODIFIED="1409838269603" TEXT="Assign sockets to Variable Nodes">
<richcontent TYPE="NOTE"><html>
  <head>
    
  </head>
  <body>
    <p>
      &#160;&#160;&#160;&#160;for (int i = 0; lambda_degs[i] != -1; i++)&#160;&#160;// Loop through all left-degrees
    </p>
    <p>
      &#160;&#160;&#160;&#160;{
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;int count_nodes_of_degree = (int) floor( lambda_wts[i] * E / lambda_degs[i] ); // No. nodes of required degree
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;for (int j = 0; j &lt; count_nodes_of_degree; j++) // Number of nodes with required left-degree
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;{
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;for (int k = 0; k &lt; lambda_degs[i]; k++) // Number of sockets for each degree
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;left_sockets[socket_index++] = &amp;variable_nodes[node_index];
    </p>
    <p>
      
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;variable_nodes[node_index].AllocateEdges(EdgeStackPointer, lambda_degs[i]);
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;variable_nodes[node_index].SetMapInUse(MapInUse);
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;variable_nodes[node_index].SetID(node_index);
    </p>
    <p>
      
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;node_index++;
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;}
    </p>
    <p>
      &#160;&#160;&#160;&#160;}
    </p>
  </body>
</html>
</richcontent>
<node CREATED="1409834848322" ID="ID_211635836" MODIFIED="1409837298082" TEXT="loop trough all left degrees lamda_degs[i]">
<node CREATED="1409834935244" ID="ID_582378038" MODIFIED="1409834950797" TEXT="Count the number of nodes with that degree"/>
<node CREATED="1409837184703" ID="ID_706028592" MODIFIED="1409837260072" TEXT="for j=0;j&lt; number of nodes with that degree">
<node CREATED="1409837311083" ID="ID_1293005225" MODIFIED="1409837337162" TEXT="for k=0;k&lt;lambda_degs[i]">
<node CREATED="1409837348309" ID="ID_719006995" MODIFIED="1409837411622" TEXT="left_sockets[socket_index++] = &amp;variable_node[node_index]"/>
</node>
<node CREATED="1409837462950" ID="ID_960389010" MODIFIED="1409837472150" TEXT="variable_nodes[node_index].AllocateEdges(EdgeStackPointer, lambda_degs[i]);">
<node CREATED="1409837523350" ID="ID_142146997" MODIFIED="1409837613710" TEXT="degree=0"/>
<node CREATED="1409837617147" ID="ID_777093449" MODIFIED="1409837628501" TEXT="edges = EdgeStackPointer"/>
<node CREATED="1409837629195" ID="ID_1422364986" MODIFIED="1409837661942" TEXT="EdgeStackPointer += lambda_degs[i]"/>
<node CREATED="1409837664783" ID="ID_10096230" MODIFIED="1409837686840" TEXT="Max_Edges = lambda_degs[i]"/>
</node>
<node CREATED="1409837472316" ID="ID_1485081173" MODIFIED="1409837479844" TEXT="variable_nodes[node_index].SetMapInUse(MapInUse);"/>
<node CREATED="1409837489844" ID="ID_946165112" MODIFIED="1409837499762" TEXT="variable_nodes[node_index].SetID(node_index);"/>
<node CREATED="1409837480433" ID="ID_1955189272" MODIFIED="1409837518688" TEXT="node_index++;"/>
</node>
</node>
<node CREATED="1409834951868" ID="ID_1517226459" MODIFIED="1409837832483" TEXT="Record no. of left sockets, may be different than E due to the fact that lambda * E may not be whole numbers ">
<richcontent TYPE="NOTE"><html>
  <head>
    
  </head>
  <body>
    <p>
      int count_left_sockets = socket_index;
    </p>
  </body>
</html>
</richcontent>
</node>
<node CREATED="1409837833898" ID="ID_869698779" MODIFIED="1409837863213" TEXT="Modify N for same reason ">
<richcontent TYPE="NOTE"><html>
  <head>
    
  </head>
  <body>
    <p>
      &#160;&#160;&#160;&#160;N = node_index;
    </p>
  </body>
</html>
</richcontent>
</node>
</node>
<node CREATED="1409837936822" FOLDED="true" ID="ID_1096899693" MODIFIED="1409838275701" TEXT="Assign sockets to Check Nodes">
<node CREATED="1409837964493" ID="ID_796208261" MODIFIED="1409837995988" TEXT="Loop trough all right degrees rho_degs[i]">
<node CREATED="1409838020209" ID="ID_1547729814" MODIFIED="1409838033569" TEXT="Count number of nodes with that degree"/>
<node CREATED="1409837184703" ID="ID_10033044" MODIFIED="1409837260072" TEXT="for j=0;j&lt; number of nodes with that degree">
<node CREATED="1409837311083" ID="ID_1667995872" MODIFIED="1409838096529" TEXT="for k=0;k&lt;rho_degs[i]">
<node CREATED="1409837348309" ID="ID_1500731863" MODIFIED="1409838090155" TEXT="right_sockets[socket_index++] = &amp;check_node[node_index]"/>
</node>
<node CREATED="1409837462950" ID="ID_120355186" MODIFIED="1409838123830" TEXT="check_nodes[node_index].AllocateEdges(EdgeStackPointer, rho_degs[i]);">
<node CREATED="1409837523350" ID="ID_67727746" MODIFIED="1409837613710" TEXT="degree=0"/>
<node CREATED="1409837617147" ID="ID_1739915208" MODIFIED="1409837628501" TEXT="edges = EdgeStackPointer"/>
<node CREATED="1409837629195" ID="ID_704546071" MODIFIED="1409838130932" TEXT="EdgeStackPointer += rho_degs[i]"/>
<node CREATED="1409837664783" ID="ID_1427334084" MODIFIED="1409838141376" STYLE="fork" TEXT="Max_Edges = rho_degs[i]"/>
</node>
<node CREATED="1409837489844" ID="ID_680581346" MODIFIED="1409838155926" TEXT="check_nodes[node_index].SetID(node_index);"/>
<node CREATED="1409837480433" ID="ID_525908423" MODIFIED="1409837518688" TEXT="node_index++;"/>
</node>
</node>
<node CREATED="1409838163226" ID="ID_83827051" MODIFIED="1409838192839" TEXT="Record no. of right sockets, may be different than E due to the fact that rho * E may not be whole numbers "/>
<node CREATED="1409838242743" ID="ID_1882613622" MODIFIED="1409838260319" TEXT="Modify M,E for same reason">
<richcontent TYPE="NOTE"><html>
  <head>
    
  </head>
  <body>
    <p>
      M = node_index;
    </p>
    <p>
      &#160;&#160;&#160;&#160;E = min(count_left_sockets, count_right_sockets);
    </p>
  </body>
</html>
</richcontent>
</node>
</node>
<node CREATED="1409838276700" ID="ID_316327796" MODIFIED="1409839169545" TEXT="Generate permutations">
<richcontent TYPE="NOTE"><html>
  <head>
    
  </head>
  <body>
    <p>
      &#160;&#160;&#160;&#160;srand(time(NULL)); // Init random seed so that each call to function returns different set of values
    </p>
    <p>
      
    </p>
    <p>
      &#160;&#160;&#160;&#160;cout &lt;&lt; &quot;Starting bipartite graph...&quot;;
    </p>
    <p>
      
    </p>
    <p>
      &#160;&#160;&#160;&#160;int left_index, right_index;
    </p>
    <p>
      &#160;&#160;&#160;&#160;for (int i = 0; i &lt; E; i++) {
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;// Randomly select socket from first E - left_index (last left_index sockets represent
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;// sockets that have already been selected)
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;//&#160;&#160;&#160;&#160;&#160;&#160;cout &lt;&lt; &quot; i = &quot; &lt;&lt; i &lt;&lt; &quot; E = &quot; &lt;&lt; E;
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;int attempts = 0;
    </p>
    <p>
      
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;do {
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;// It is important to select left_index randomly and not only right_index,
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;// because there is a significance to the order of bits within a code, sometimes with
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;// first or last bits better protected.&#160;&#160;If left_index is not selected randomly,
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;// the result would be a random tree, but in which lower degree left-nodes are of
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;// lower index within each constituent code, contrary to complete randomness.
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;left_index = uniform_random(E - i);
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;//left_index = i;
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;right_index = uniform_random(E - i);
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;if ((attempts++) &gt; 100) {
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;cout &lt;&lt; &quot;Warning: cycles\n&quot;;
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;break;
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;}
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;} while (left_sockets[left_index]-&gt;IsRightConnectedTo(
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;right_sockets[right_index]));
    </p>
    <p>
      
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;if (right_index &gt;= (E - i)) {
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;cout &lt;&lt; &quot;right index or left_index exceed range\n&quot;
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&lt;&lt; &quot;right_index = &quot; &lt;&lt; right_index &lt;&lt; &quot; left_index = &quot;
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&lt;&lt; left_index &lt;&lt; &quot; E = &quot; &lt;&lt; E &lt;&lt; &quot; left_index = &quot;
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&lt;&lt; left_index &lt;&lt; &quot;\n&quot;;
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;exit(1);
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;}
    </p>
    <p>
      
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;edges[i].set_nodes(left_sockets[left_index],
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;right_sockets[right_index]);
    </p>
    <p>
      
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;// Overwrite current sockets with last sockets, so that they are not selected again
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;right_sockets[right_index] = right_sockets[E - i - 1];
    </p>
    <p>
      &#160;&#160;&#160;&#160;&#160;&#160;&#160;&#160;left_sockets[left_index] = left_sockets[E - i - 1];
    </p>
    <p>
      &#160;&#160;&#160;&#160;}
    </p>
    <p>
      
    </p>
    <p>
      &#160;&#160;&#160;&#160;cout &lt;&lt; &quot;Done\n&quot;;
    </p>
    <p>
      
    </p>
    <p>
      &#160;&#160;&#160;&#160;// Clean-up
    </p>
    <p>
      &#160;&#160;&#160;&#160;delete left_sockets;
    </p>
    <p>
      &#160;&#160;&#160;&#160;delete right_sockets;
    </p>
  </body>
</html>
</richcontent>
</node>
</node>
<node CREATED="1409737247588" ID="ID_808911292" MODIFIED="1409737249688" TEXT="Variables.Init(Graph.variable_nodes, Graph.N);"/>
<node CREATED="1409737252116" ID="ID_1251313932" MODIFIED="1409737271173" TEXT="Checks.Init(Graph.check_nodes, Graph.M);"/>
</node>
<node CREATED="1409638889634" ID="ID_696602883" MODIFIED="1409641148850" POSITION="right" TEXT="Code.GetZeroCodeword(Codeword);">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409638900814" ID="ID_214529080" MODIFIED="1409641148850" POSITION="right" STYLE="fork" TEXT="Channel-&gt;SimulateOutputVector(Codeword, ChannelOutput);">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409638925762" ID="ID_1699898478" MODIFIED="1409641148850" POSITION="right" TEXT="Code.Init_Messages(ChannelOutput);">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409638927498" ID="ID_1163776652" MODIFIED="1409641148850" POSITION="right" STYLE="fork" TEXT="AccumulatedSER += Code.Belief_Propagation_Decoder(iterations);">
<font NAME="SansSerif" SIZE="12"/>
</node>
</node>
</map>
