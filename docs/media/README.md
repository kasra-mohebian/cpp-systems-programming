# Technical project illustration

`project-overview-technical.png` was generated with the built-in OpenAI image-generation tool in Codex. It is conceptual artwork, not a screenshot, measured chart, hardware photograph or a figure taken from a paper or teaching material.

Technical concept illustration of ownership, collision chains and atomic publication; object shapes are schematic, not source-object layouts.

The image follows MIT for original explanatory material, to the extent rights exist and are held. No exclusive rights are asserted in legally unprotectable material; third-party rights are unaffected.

## Technical diagram

`implementation-map.svg`: The original code-drawn diagram explains AST ownership, collision chains, resource lifetime and release/acquire publication. The same original-material terms apply.

## Generation prompt

> Use case: stylized-concept. Asset: wide technical editorial cover for a serious GitHub engineering portfolio. Create a richly detailed, professional technical illustration, not a generic cute infographic. Visual language: precision CAD-like cutaways blended with sophisticated scientific magazine illustration; deep ink/navy structures, luminous teal pathways, restrained amber highlights, pale neutral background, subtle alignment grid, clear hierarchy and depth. Wide landscape around 2:1. Fine engineering details must serve the explanation, not random ornament. No people, university/publisher marks, brand logos, watermarks, fake benchmark numbers, charts or product screenshots. All spatial layouts are conceptual schematics. Use only the few exact labels requested, with crisp typography. Subject: actual C++ systems-programming mechanisms arranged as a single engineering workbench. Main focus is a precise abstract syntax tree with exclusive-ownership links to child objects, beside a hash-table bucket array with well-defined linked collision chains. A lower inset shows a scoped temporary resource entering ownership, moving once, and being released when the scope ends. Another inset shows a writer publishing a completed immutable node to readers across an atomic boundary; no concurrent deletion. Visualize concrete memory cells, pointer links, object boundaries, lock versus atomic motifs and orderly life-cycle trajectories. No impossible cross-links, fabricated code listings or numerical performance claims. Exact labels only: 'RAII', 'AST', 'HASH TABLE', 'ATOMIC PUBLISH'.

### Refinement 1

> Refine this technical illustration, preserving its professional style, palette, panel arrangement and detailed isometric rendering. Correct the AST panel: it MUST be a strict tree with exactly one root, two child nodes, and four leaf nodes; every child/leaf has exactly ONE incoming parent ownership link; NO shared child, NO cycle, NO bidirectional ownership arrows. Links must point only from parent to child. In the HASH TABLE panel, bucket pointers and next pointers must be unidirectional from bucket down its own chain, ending at nullptr; remove bidirectional arrows. In ATOMIC PUBLISH, show a single completed immutable node referenced by multiple reader arrows, rather than copies being made for each reader. RAII should clearly read acquire -> own -> move -> source empty -> use -> release, without suggesting a destroyed resource is used again. This remains a conceptual illustration, not a literal layout of source structs. Keep headings AST, HASH TABLE, RAII, ATOMIC PUBLISH, no new claims or benchmarks.

### Refinement 2

> Keep this illustration otherwise exactly the same. In the upper AST and HASH TABLE panels only, REMOVE ALL ARROWHEADS from the cyan and amber connection lines, including the upward arrowheads near parent ports and bucket ports, leaving clean single connection paths without direction markers. Keep the strict binary tree with no shared children and the separate hash collision chains. Do not redraw nodes, headings, fields or the lower RAII/atomic panels. These upper connections must be neutral connectors, not misleading bidirectional pointer arrows. No other changes.
