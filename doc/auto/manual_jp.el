(TeX-add-style-hook "manual_jp"
 (lambda ()
    (LaTeX-add-environments
     "Japanese")
    (LaTeX-add-labels
     "sec:intro"
     "sec:requirements"
     "sec:installation"
     "sec:configuration"
     "tab:config_params"
     "sec:port"
     "sec1:naming"
     "tab:port_naming"
     "sec:model-proxies"
     "tab:proxies"
     "sec1:filtering-models"
     "tab:filters"
     "tab:filtered_proxies"
     "sec1:proxy-plugins"
     "sec1:example-plugins"
     "sec2:blobfinder-proxy"
     "sec2:position-proxy")
    (TeX-add-symbols
     '("ilcode" 1))
    (TeX-run-style-hooks
     "booktabs"
     "tabularx"
     "a4wide"
     "listings"
     "CJKulem"
     "ruby"
     "overlap"
     "CJK"
     "CJKutf8"
     "latex2e"
     "art10"
     "article"
     "a4paper"
     "10pt")))

